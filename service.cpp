/*
 * Copyright (C) 2019 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "vendor.lineage.livedisplay@2.0-service.nvidia"

#include <android-base/logging.h>
#include <binder/ProcessState.h>
#include <hidl/HidlTransportSupport.h>

#include "LiveDisplay.h"
#include "DisplayColorCalibration.h"
#include "ReadingEnhancement.h"

using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
using android::sp;
using android::status_t;
using android::OK;

using vendor::lineage::livedisplay::V2_0::nvidia::LiveDisplay;
using vendor::lineage::livedisplay::V2_0::nvidia::DisplayColorCalibration;
using vendor::lineage::livedisplay::V2_0::nvidia::ReadingEnhancement;

int main() {
    std::shared_ptr<LiveDisplay> liveDisplay;
    sp<DisplayColorCalibration> displayColorCalibration;
    sp<ReadingEnhancement> readingEnhancement;
    status_t status;

    LOG(INFO) << "LiveDisplay HAL service is starting.";

    liveDisplay = std::make_shared<LiveDisplay>();
    if (liveDisplay == nullptr) {
        LOG(ERROR)
            << "Can not create an instance of LiveDisplay HAL LiveDisplay Iface, exiting.";
        goto shutdown;
    }

    displayColorCalibration = new DisplayColorCalibration(liveDisplay);
    if (displayColorCalibration == nullptr) {
        LOG(ERROR) << "Can not create an instance of LiveDisplay HAL DisplayColorCalibration "
                      "Iface, exiting.";
        goto shutdown;
    }

    readingEnhancement = new ReadingEnhancement(liveDisplay);
    if (readingEnhancement == nullptr) {
        LOG(ERROR)
            << "Can not create an instance of LiveDisplay HAL ReadingEnhancement Iface, exiting.";
        goto shutdown;
    }

    configureRpcThreadpool(1, true /*callerWillJoin*/);

    if (displayColorCalibration->isSupported()) {
        status = displayColorCalibration->registerAsService();
        if (status != OK) {
            LOG(ERROR)
                << "Could not register service for LiveDisplay HAL DisplayColorCalibration Iface ("
                << status << ")";
            goto shutdown;
        }
    }

    if (readingEnhancement->isSupported()) {
        status = readingEnhancement->registerAsService();
        if (status != OK) {
            LOG(ERROR)
                << "Could not register service for LiveDisplay HAL ReadingEnhancement Iface ("
                << status << ")";
            goto shutdown;
        }
    }

    LOG(INFO) << "LiveDisplay HAL service is ready.";
    joinRpcThreadpool();
// Should not pass this line

shutdown:
    // In normal operation, we don't expect the thread pool to shutdown
    LOG(ERROR) << "LiveDisplay HAL service is shutting down.";
    return 1;
}
