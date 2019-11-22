﻿// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H
#pragma warning(disable : 4251 4275 26495 26451 6387 26439 26498 26495 26498 6387 26110 26451)
// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"
#include <Windows.h>
#include <iostream>
#include <vector>
#include <list>
#include <chrono>
#include <memory>
#include <ppl.h>
#include <ppltasks.h>
#include <map>
#include <queue>
#include <unordered_map>
#include <mutex>
#include <fstream>
#include <string>
#include <map>
#include <functional>
#include <algorithm>
#include <ostream>
#include <istream>
#include <filesystem>
#include  "psapi.h"
#include <Pdh.h>
#include <PdhMsg.h>

namespace fs = std::experimental::filesystem;

#endif //PCH_H
