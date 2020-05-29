/*
 * Copyright (C) 2020 The LineageOS Project
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

#ifndef VENDOR_LINEAGE_LIVEDISPLAY_V2_0_LIVEDISPLAY_H
#define VENDOR_LINEAGE_LIVEDISPLAY_V2_0_LIVEDISPLAY_H

#include <map>
#include <vector>

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace nvidia {

class LiveDisplay {
  public:
    LiveDisplay();
    bool IsCMUSupported();
    std::vector<uint16_t> const GetCMU();
    bool SetCMU(std::vector<uint16_t> const &csc);

  private:
    std::map<uint8_t, bool> mDisplayControllers;
};

}  // namespace nvidia
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor

#endif  // VENDOR_LINEAGE_LIVEDISPLAY_V2_0_LIVEDISPLAY_H
