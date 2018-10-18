#pragma once

namespace MPI
{
	const int MasterRank = 0;

	class Process
	{
	public:
		Process();
		~Process();

		int ProcessCount() const;
		int Rank() const;
		bool IsMaster() const;
	private:
		int process_count_;
		int rank_;
	};
}

