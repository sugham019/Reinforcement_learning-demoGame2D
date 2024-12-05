#pragma once
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <string>

enum class Reward: int{

    DEAD = -25,
    SPIKE_BLASTED = 25,
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
    } *m_sharedData = nullptr;

    std::string m_sharedRegion;
};