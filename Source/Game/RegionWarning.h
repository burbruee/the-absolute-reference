#pragma once

typedef enum RegionWarningState {
	REGIONWARNING_JAPAN,
	REGIONWARNING_SKIP,
	REGIONWARNING_KOREA,
	REGIONWARNING_REGIONSETTING
} RegionWarningState;
extern RegionWarningState RegionWarning;

