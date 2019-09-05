#ifndef BITCOIN_TEST_TEST_VULCOIN_H
#define BITCOIN_TEST_TEST_VULCOIN_H

#include "txdb.h"

#include <boost/filesystem.hpp>
#include <boost/thread.hpp>

struct TestingSetup {
    CCoinsViewDB *pcoinsdbview;
    boost::filesystem::path pathTemp;
    boost::thread_group threadGroup;
    ECCVerifyHandle globalVerifyHandle;

    TestingSetup();
    ~TestingSetup();
};

#endif