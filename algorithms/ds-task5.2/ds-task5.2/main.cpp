#define _CRT_SECURE_NO_WARNINGS

#include <queue>
#include <vector>
#include <memory>
#include <stdint.h>
#include <stdio.h>

using namespace std;

struct BusStop
{
	int stop_number;
	int16_t number_in_route;
	int16_t route;
	bool blocked;

	BusStop(int stop_number, int16_t number_in_route, int16_t route) : stop_number(stop_number), route(route),
		number_in_route(number_in_route), blocked(false) {}
};

struct BusStopQ
{
	shared_ptr<BusStop> stop;
	shared_ptr<BusStopQ> previous_stop;
	int length;

	BusStopQ(shared_ptr<BusStop> stop, shared_ptr<BusStopQ> previous_stop, int length) : stop(stop), 
		previous_stop(previous_stop),
		length(length) {}
};

int main()
{
	FILE *fin = fopen("in.txt", "r");
	FILE *fout = fopen("out.txt", "w");
	  
	int number_of_stops;
	int16_t number_of_routes;
	fscanf(fin, "%d %hd", &number_of_stops, &number_of_routes);

	int first_stop, last_stop; 
	fscanf(fin, "%d %d", &first_stop, &last_stop);

	last_stop--;
	first_stop--;
	if (last_stop == first_stop)
	{
		fprintf(fout, "0");
		fclose(fout);
		fclose(fin);
		return 0;
	}

	queue<shared_ptr<BusStopQ>> queue_a; // если пришли по дуге длины a = 1
	queue<shared_ptr<BusStopQ>> queue_b; // если пришли по дуге длины b = 3

	// для каждой остановки храним следующие остановки и маршруты, по которым можем к ним проехать, если
	// по одному маршруту можно проехать до нескольких остановок, то до первой можем проехать за 1, а до следующей
	// сначала потратив 3 за выход/вход в автобус того же маршрута
	vector<vector<shared_ptr<BusStop>>> g(static_cast<size_t>(number_of_stops), vector<shared_ptr<BusStop>>());

	shared_ptr<BusStop> current_stop_ptr = nullptr, next_stop_ptr = nullptr;
	for (int16_t i = 0; i < number_of_routes; i++)
	{
		int16_t n;
		fscanf(fin, "%hd", &n);

		int current_stop;
		fscanf(fin, "%d", &current_stop);

		current_stop_ptr = make_shared<BusStop>(current_stop - 1, 0, i);
		if (current_stop - 1 == first_stop)
		{
			queue_a.push(make_shared<BusStopQ>(current_stop_ptr, nullptr, 0));
		}

		for (int16_t j = 0; j < n - 1; j++)
		{
			int next_stop;
			fscanf(fin, "%d", &next_stop);

			next_stop_ptr = make_shared<BusStop>(next_stop - 1, j + 1, i);

			if (next_stop - 1 == first_stop)
			{
				queue_a.push(make_shared<BusStopQ>(next_stop_ptr, nullptr, 0));
			}

			g[current_stop - 1].push_back(next_stop_ptr);
			g[next_stop - 1].push_back(current_stop_ptr);

			current_stop = next_stop;
			current_stop_ptr = next_stop_ptr;
		}
	}

	shared_ptr<BusStopQ> last_stop_counted = nullptr;
	shared_ptr<BusStopQ> blocked_stop = nullptr;
	while (!queue_a.empty() || !queue_b.empty())
	{
		if (queue_a.empty() || (!queue_b.empty() && queue_b.front()->length < queue_a.front()->length))
		{
			blocked_stop = queue_b.front();
			queue_b.pop();
		}
		else if (queue_b.empty() || queue_a.front()->length <= queue_b.front()->length)
		{
			blocked_stop = queue_a.front();
			queue_a.pop();
		}

		if (blocked_stop->stop->stop_number == last_stop)
		{
			last_stop_counted = blocked_stop;
			break;
		}

		if (!blocked_stop->stop->blocked)
		{
			blocked_stop->stop->blocked = true;

			for (size_t i = 0; i < g.at(blocked_stop->stop->stop_number).size(); i++)
			{
				auto next_stop = g.at(blocked_stop->stop->stop_number).at(i);
				if (!next_stop->blocked)
				{
					if (next_stop->route == blocked_stop->stop->route &&
						abs(next_stop->number_in_route - blocked_stop->stop->number_in_route) == 1)
					{
						queue_a.push(make_shared<BusStopQ>(next_stop, blocked_stop, blocked_stop->length + 1));
					}
					else
					{
						queue_b.push(make_shared<BusStopQ>(next_stop, blocked_stop, blocked_stop->length + 4));
					}
				}
			}
		}
	}

	if (last_stop_counted == nullptr)
		fprintf(fout, "NoWay");
	else
	{
		fprintf(fout, "%d\n", last_stop_counted->length);

		for (shared_ptr<BusStopQ> stop = last_stop_counted; stop != nullptr; stop = stop->previous_stop)
		{
			fprintf(fout, "%d %d\n", stop->stop->stop_number + 1, stop->stop->route + 1);
		}
	}

	fclose(fout);
	fclose(fin);
	return 0;
}