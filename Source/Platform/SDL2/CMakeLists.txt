set(EXECUTABLE_SOURCES
	"../../BuildData/BuildData.c"
	"../../BuildData/Checksum.c"

	"../../Eeprom/Eeprom.c"
	"../../Eeprom/Setting.c"

	"../../Game/RegionWarning.c"
	"../../Game/Save.c"
	"../../Game/Screen.c"
	"../../Game/Sequence.c"
	"../../Game/Temp.c"

	"../../Game/Graphics/DisplayObject.c"
	"../../Game/Graphics/Entity.c"
	"../../Game/Graphics/ShowBlockField.c"
	"../../Game/Graphics/ShowClearTime.c"
	"../../Game/Graphics/ShowColumnMarker.c"
	"../../Game/Graphics/ShowEffect.c"
	"../../Game/Graphics/ShowGameOverFade.c"
	"../../Game/Graphics/ShowGameStatus.c"
	"../../Game/Graphics/ShowNewChallenger.c"
	"../../Game/Graphics/ShowObject.c"
	"../../Game/Graphics/ShowRowDel.c"
	"../../Game/Graphics/ShowStaff.c"
	"../../Game/Graphics/ShowText.c"

	"../../Game/Play/Debug.c"
	"../../Game/Play/DelayTable.c"
	"../../Game/Play/Grade.c"
	"../../Game/Play/GravityTable.c"
	"../../Game/Play/Medal.c"
	"../../Game/Play/Player.c"
	"../../Game/Play/Ranking.c"
	"../../Game/Play/RankingCode.c"
	"../../Game/Play/Versus.c"

	"../../Game/Play/Item/Item.c"
	"../../Game/Play/Item/ShowItemEffect.c"
	"../../Game/Play/Item/ShowItemWarningColorBlock.c"
	"../../Game/Play/Item/ShowItemWarningDarkBlock.c"
	"../../Game/Play/Item/ShowItemWarningDeathBlock.c"
	"../../Game/Play/Item/ShowItemWarningExchgField.c"
	"../../Game/Play/Item/ShowItemWarningHardBlock.c"
	"../../Game/Play/Item/ShowItemWarningMirrorBlock.c"
	"../../Game/Play/Item/ShowItemWarningNegaField.c"
	"../../Game/Play/Item/ShowItemWarningRollRoll.c"
	"../../Game/Play/Item/ShowItemWarningShotgun.c"
	"../../Game/Play/Item/ShowItemWarningTransform.c"
	"../../Game/Play/Item/ShowItemWarningXRay.c"
	"../../Game/Play/Item/ShowLaser.c"
	"../../Game/Play/Item/UpdateItemBottomDelField.c"
	"../../Game/Play/Item/UpdateItemColorBlock.c"
	"../../Game/Play/Item/UpdateItemDarkBlock.c"
	"../../Game/Play/Item/UpdateItemDeathBlock.c"
	"../../Game/Play/Item/UpdateItemDelEven.c"
	"../../Game/Play/Item/UpdateItemExchgField.c"
	"../../Game/Play/Item/UpdateItemFlipField.c"
	"../../Game/Play/Item/UpdateItemFreeFall.c"
	"../../Game/Play/Item/UpdateItemHardBlock.c"
	"../../Game/Play/Item/UpdateItemLaser.c"
	"../../Game/Play/Item/UpdateItemLeftMovField.c"
	"../../Game/Play/Item/UpdateItemMirrorBlock.c"
	"../../Game/Play/Item/UpdateItemNegaField.c"
	"../../Game/Play/Item/UpdateItemRightMovField.c"
	"../../Game/Play/Item/UpdateItemRollRoll.c"
	"../../Game/Play/Item/UpdateItemShotgun.c"
	"../../Game/Play/Item/UpdateItemTopDelField.c"
	"../../Game/Play/Item/UpdateItemTransform.c"
	"../../Game/Play/Item/UpdateItemXRay.c"

	"../../Input/Button.c"
	"../../Input/Coin.c"
	"../../Input/Credit.c"
	"../../Input/Input.c"

	"../../Lib/LibC.c"
	"../../Lib/Math.c"

	"../../Main/DemoLoop.c"
	"../../Main/Frame.c"
	"../../Main/GameLoop.c"
	"../../Main/Main.c"
	"../../Main/MainLoop.c"
	"../../Main/TestLoop.c"

	"../../Sound/Sound.c"

	"../../Video/BgMap.c"
	"../../Video/GameBg.c"
	"../../Video/InitScaleRam.c"
	"../../Video/Pal.c"
	"../../Video/SpriteInit.c"
	"../../Video/UnknownSprite.c"
	"../../Video/Video.c"

	"../Util/HwData.c"
	"../Util/RomData.c"

	"PlatformUpdate.c"
)
set(EXECUTABLE_HEADERS
	"../../BuildData/BuildData.h"
	"../../BuildData/Checksum.h"

	"../../Eeprom/Eeprom.h"
	"../../Eeprom/Setting.h"

	"../../Game/RegionWarning.h"
	"../../Game/Save.h"
	"../../Game/Screen.h"
	"../../Game/Sequence.h"
	"../../Game/Temp.h"

	"../../Game/Graphics/DisplayObject.h"
	"../../Game/Graphics/Entity.h"
	"../../Game/Graphics/ShowBlockField.h"
	"../../Game/Graphics/ShowClearTime.h"
	"../../Game/Graphics/ShowColumnMarker.h"
	"../../Game/Graphics/ShowEffect.h"
	"../../Game/Graphics/ShowGameOverFade.h"
	"../../Game/Graphics/ShowGameStatus.h"
	"../../Game/Graphics/ShowNewChallenger.h"
	"../../Game/Graphics/ShowObject.h"
	"../../Game/Graphics/ShowRowDel.h"
	"../../Game/Graphics/ShowStaff.h"
	"../../Game/Graphics/ShowText.h"

	"../../Game/Play/Debug.h"
	"../../Game/Play/DelayTable.h"
	"../../Game/Play/Grade.h"
	"../../Game/Play/GravityTable.h"
	"../../Game/Play/Medal.h"
	"../../Game/Play/Player.h"
	"../../Game/Play/Ranking.h"
	"../../Game/Play/RankingCode.h"
	"../../Game/Play/Versus.h"

	"../../Game/Play/Item/Item.h"
	"../../Game/Play/Item/ShowItemEffect.h"
	"../../Game/Play/Item/ShowItemWarningColorBlock.h"
	"../../Game/Play/Item/ShowItemWarningDarkBlock.h"
	"../../Game/Play/Item/ShowItemWarningDeathBlock.h"
	"../../Game/Play/Item/ShowItemWarningExchgField.h"
	"../../Game/Play/Item/ShowItemWarningHardBlock.h"
	"../../Game/Play/Item/ShowItemWarningMirrorBlock.h"
	"../../Game/Play/Item/ShowItemWarningNegaField.h"
	"../../Game/Play/Item/ShowItemWarningRollRoll.h"
	"../../Game/Play/Item/ShowItemWarningShotgun.h"
	"../../Game/Play/Item/ShowItemWarningTransform.h"
	"../../Game/Play/Item/ShowItemWarningXRay.h"
	"../../Game/Play/Item/ShowLaser.h"
	"../../Game/Play/Item/UpdateItemBottomDelField.h"
	"../../Game/Play/Item/UpdateItemColorBlock.h"
	"../../Game/Play/Item/UpdateItemDarkBlock.h"
	"../../Game/Play/Item/UpdateItemDeathBlock.h"
	"../../Game/Play/Item/UpdateItemDelEven.h"
	"../../Game/Play/Item/UpdateItemExchgField.h"
	"../../Game/Play/Item/UpdateItemFlipField.h"
	"../../Game/Play/Item/UpdateItemFreeFall.h"
	"../../Game/Play/Item/UpdateItemHardBlock.h"
	"../../Game/Play/Item/UpdateItemLaser.h"
	"../../Game/Play/Item/UpdateItemLeftMovField.h"
	"../../Game/Play/Item/UpdateItemMirrorBlock.h"
	"../../Game/Play/Item/UpdateItemNegaField.h"
	"../../Game/Play/Item/UpdateItemRightMovField.h"
	"../../Game/Play/Item/UpdateItemRollRoll.h"
	"../../Game/Play/Item/UpdateItemShotgun.h"
	"../../Game/Play/Item/UpdateItemTopDelField.h"
	"../../Game/Play/Item/UpdateItemTransform.h"
	"../../Game/Play/Item/UpdateItemXRay.h"

	"../../Input/Button.h"
	"../../Input/Coin.h"
	"../../Input/Credit.h"
	"../../Input/Input.h"
	"../../Input/InputTypes.h"

	"../../Lib/LibC.h"
	"../../Lib/Macros.h"
	"../../Lib/Math.h"

	"../../Main/DemoLoop.h"
	"../../Main/Frame.h"
	"../../Main/GameLoop.h"
	"../../Main/Main.h"
	"../../Main/MainLoop.h"
	"../../Main/TestLoop.h"

	"../../Sound/Sound.h"

	"../../Video/BgMap.h"
	"../../Video/InitScaleRam.h"
	"../../Video/GameBg.h"
	"../../Video/Pal.h"
	"../../Video/SpriteInit.h"
	"../../Video/UnknownSprite.h"
	"../../Video/Video.h"

	"../Util/Color.h"
	"../Util/HwData.h"
	"../Util/Macros.h"

	"PlatformSupport/Config.h"
	"PlatformSupport/HwData.h"
	"PlatformSupport/PlatformUpdate.h"
	"PlatformSupport/PlatformTypes.h"
)

add_executable("${EXECUTABLE_NAME}" ${EXECUTABLE_SOURCES} ${EXECUTABLE_HEADERS})
source_group("Header Files" FILES ${EXECUTABLE_HEADERS})
target_include_directories("${EXECUTABLE_NAME}" PUBLIC "../.." "../../Generated" "PlatformSupport")

if (VCPKG_TOOLCHAIN)
	set_property(
		TARGET "${EXECUTABLE_NAME}"
		PROPERTY VS_DEBUGGER_WORKING_DIRECTORY
		${CMAKE_SOURCE_DIR}
	)
endif()

find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
	pkg_check_modules(SDL2 REQUIRED sdl2)
	target_compile_options(${EXECUTABLE_NAME} PRIVATE "${SDL2_CFLAGS}")
	target_compile_options(${EXECUTABLE_NAME} PRIVATE "${SDL2_CFLAGS_OTHER}")
	target_include_directories(${EXECUTABLE_NAME} SYSTEM PRIVATE "${SDL2_INCLUDE_DIRS}")
	target_link_directories(${EXECUTABLE_NAME} PRIVATE "${SDL2_LIBRARY_DIRS}")
	target_link_libraries(${EXECUTABLE_NAME} PRIVATE "${SDL2_LIBRARIES}")
	target_link_options(${EXECUTABLE_NAME} PRIVATE "${SDL2_LDFLAGS}")
	target_link_options(${EXECUTABLE_NAME} PRIVATE "${SDL2_LDFLAGS_OTHER}")
elseif(VCPKG_TOOLCHAIN)
	set(VCPKG_CRT_LINKAGE dynamic)
	set(VCPKG_LIBRARY_LINKAGE dynamic)

	find_package(SDL2 REQUIRED)
	target_link_libraries(${EXECUTABLE_NAME}
		PRIVATE
			SDL2::SDL2
			SDL2::SDL2main
	)
endif()

install(TARGETS "${EXECUTABLE_NAME}" RUNTIME)
