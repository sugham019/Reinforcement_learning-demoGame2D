#include "RewardSender.hpp"
#include "ErrorHandler.hpp"
#include <boost/interprocess/creation_tags.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/detail/os_file_functions.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <exception>

using namespace boost::interprocess;

RewardSender::RewardSender(const std::string& sharedRegion): m_sharedRegion(sharedRegion.c_str()){
    try {
        m_sharedMemoryObject = new shared_memory_object(create_only, sharedRegion.c_str(), read_write);
        m_sharedMemoryObject->truncate(sizeof(SharedData));
        m_mappedRegionInMemory= new mapped_region(*m_sharedMemoryObject, read_write);

    } catch (std::exception exception) {
        reportError(exception.what());
    }
    void* sharedData = m_mappedRegionInMemory->get_address();
    m_sharedData = static_cast<SharedData*>(sharedData);

}

RewardSender::~RewardSender(){
    shared_memory_object::remove(m_sharedRegion);
    delete m_sharedMemoryObject;
    delete m_mappedRegionInMemory;
}

void RewardSender::sendReward(const Reward& reward){
    SharedData& sharedData = *m_sharedData;
    sharedData.reward = static_cast<int>(reward);
    while(sharedData.acknowledgement != 1);
    sharedData.acknowledgement = 0;
}