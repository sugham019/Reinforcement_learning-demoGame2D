#pragma once
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <string>

enum class Reward: int{

    DEAD = -5,
    SUCCESSFUL_JUMP = 5,
    SPIKE_BLASTED = 3
};

class RewardSender{

public:
    RewardSender(const std::string& sharedRegion);
    ~RewardSender();
    void sendReward(const Reward& reward);

private:
    boost::interprocess::shared_memory_object* m_sharedMemoryObject;
    boost::interprocess::mapped_region* m_mappedRegionInMemory;

    struct SharedData {
        int reward = 0;
        int acknowledgement = 0;
    } *m_sharedData = nullptr;

    const char* m_sharedRegion;
};