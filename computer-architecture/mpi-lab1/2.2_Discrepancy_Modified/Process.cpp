#include "Process.h"
#include <mpi.h>

namespace MPI
{
	Process::Process()
	{
		MPI_Init(nullptr, nullptr);
		MPI_Comm_size(MPI_COMM_WORLD, &process_count_);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank_);
	}

	Process::~Process()
	{
		MPI_Finalize();
	}

	int Process::ProcessCount() const
	{
		return process_count_;
	}

	int Process::Rank() const
	{
		return rank_;
	}

	bool Process::IsMaster() const
	{
		return (rank_ == MasterRank);
	}

}

