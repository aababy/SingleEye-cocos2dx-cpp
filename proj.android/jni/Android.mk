LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libSingleEye
#LOCAL_STATIC_LIBS
LOCAL_SRC_FILES := ../libs/libSingleEye_android.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_LDLIBS := -llog

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp


LOCAL_SRC_FILES := hellocpp/main.cpp \
				   hellocpp/Social.cpp \
				   hellocpp/com_gwsoft_singleeye_SingleEye.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/Scene/AboutScene.cpp \
                   ../../Classes/Scene/BossScene.cpp \
                   ../../Classes/Scene/ButtonAnimation.cpp \
                   ../../Classes/Scene/ButtonSprite.cpp \
                   ../../Classes/Scene/BuyGemScene.cpp \
                   ../../Classes/Scene/BuyGoldScene.cpp \
                   ../../Classes/Scene/BuyPowerScene.cpp \
                   ../../Classes/Scene/DefenseScene.cpp \
                   ../../Classes/Scene/DungeonScene.cpp \
                   ../../Classes/Scene/HuodongScene.cpp \
                   ../../Classes/Scene/LevelScene.cpp \
                   ../../Classes/Scene/LoadScene.cpp \
                   ../../Classes/Scene/LoginScene.cpp \
                   ../../Classes/Scene/MessageScene.cpp \
                   ../../Classes/Scene/MoneyItem.cpp \
                   ../../Classes/Scene/PlanScene.cpp \
                   ../../Classes/Scene/PvPScene.cpp \
                   ../../Classes/Scene/SelectHeroScene.cpp \
                   ../../Classes/Scene/SelectOutfitScene.cpp \
                   ../../Classes/Scene/SelectPetScene.cpp \
                   ../../Classes/Scene/ShareScene.cpp \
                   ../../Classes/Scene/ShopScene.cpp \
                   ../../Classes/Scene/SignScene.cpp \
                   ../../Classes/Scene/StoryScene.cpp \
                   ../../Classes/Scene/TavernScene.cpp \
                   ../../Classes/Scene/TitleScene.cpp \
                   ../../Classes/Scene/FightScene/FightScene.cpp \
                   ../../Classes/Scene/FightScene/ResultLayer.cpp \
                   ../../Classes/Scene/FightScene/SnatchLayer.cpp \
                   ../../Classes/Scene/homeScene/HomeAster.cpp \
                   ../../Classes/Scene/homeScene/HomeCloud.cpp \
                   ../../Classes/Scene/homeScene/HomeControlLayer.cpp \
                   ../../Classes/Scene/homeScene/HomeScene.cpp \
                   ../../Classes/Scene/homeScene/HomeUILayer.cpp \
                   ../../Classes/Scene/homeScene/MaskLayer.cpp \
                   ../../Classes/Scene/homeScene/PingLunScene.cpp \
                   ../../Classes/Scene/homeScene/SettingScene.cpp \
                   ../../Classes/Scene/homeScene/SlideMenu.cpp \
                   ../../Classes/Scene/homeScene/TeachLayer.cpp \
                   ../../Classes/Sprites/Enemy.cpp \
                   ../../Classes/Sprites/Hero.cpp \
                   ../../Classes/Sprites/Lansquenet.cpp \
                   ../../Classes/Logic/CCShake.cpp \
                   ../../Classes/Logic/State/EnemyAtkState.cpp \
                   ../../Classes/Logic/State/EnemyDodgeState.cpp \
                   ../../Classes/Logic/State/EnemyDrillOutStayState.cpp \
                   ../../Classes/Logic/State/EnemyDrillState.cpp \
                   ../../Classes/Logic/State/EnemyDrillStayState.cpp \
                   ../../Classes/Logic/State/EnemyEatState.cpp \
                   ../../Classes/Logic/State/EnemyHurtState.cpp \
                   ../../Classes/Logic/State/EnemyInkJetState.cpp \
                   ../../Classes/Logic/State/EnemyInvisibilityState.cpp \
                   ../../Classes/Logic/State/EnemyJumpState.cpp \
                   ../../Classes/Logic/State/EnemyMoveState.cpp \
                   ../../Classes/Logic/State/EnemyPoisonState.cpp \
                   ../../Classes/Logic/State/EnemyReplicateState.cpp \
                   ../../Classes/Logic/State/EnemyShieldState.cpp \
                   ../../Classes/Logic/State/EnemySlowDownState.cpp \
                   ../../Classes/Logic/State/EnemySprintState.cpp \
                   ../../Classes/Logic/State/EnemyStayState.cpp \
                   ../../Classes/Logic/State/EnemyStopState.cpp \
                   ../../Classes/Logic/State/EnemyTransferState.cpp \
                   ../../Classes/Public/AsyncLM.cpp \
                   ../../Classes/Public/Block.cpp \
                   ../../Classes/Public/CfgDataManage.cpp \
                   ../../Classes/Public/DataPool.cpp \
                   ../../Classes/Public/Date.cpp \
                   ../../Classes/Public/Dialog.cpp \
                   ../../Classes/Public/HelpMethod.cpp \
                   ../../Classes/Public/Loading.cpp \
                   ../../Classes/Public/MTNotificationQueue.cpp \
                   ../../Classes/Public/SceneManager.cpp \
                   ../../Classes/Public/ScrollText.cpp \
                   ../../Classes/Public/Share.cpp \
                   ../../Classes/Public/Timer.cpp \
                   ../../Classes/Public/TransitScene.cpp \
                   ../../Classes/Public/XmlData.cpp \
                   ../../Classes/Engine/EngineSystem/EngineSystem.cpp \
                   ../../Classes/Engine/GUISystem/CCDrawHelp.cpp \
                   ../../Classes/Engine/GUISystem/CCTPSprite.cpp \
                   ../../Classes/Engine/ScriptSystem/LuaManager.cpp \
                   ../../Classes/Engine/ScriptSystem/LuaScript.cpp \
                   ../../Classes/Engine/NetSystem/NetWorker.h \
                   ../../Classes/Engine/DesSystem/DesSystem.cpp \
                   ../../../../scripting/lua/lua/lua.c \
                   ../../../../scripting/lua/lua/lauxlib.c \
                   ../../../../scripting/lua/lua/lapi.c \
                   ../../../../scripting/lua/lua/lbaselib.c \
                   ../../../../scripting/lua/lua/lcode.c \
                   ../../../../scripting/lua/lua/ldblib.c \
                   ../../../../scripting/lua/lua/ldebug.c \
                   ../../../../scripting/lua/lua/ldo.c \
                   ../../../../scripting/lua/lua/ldump.c \
                   ../../../../scripting/lua/lua/lfunc.c \
                   ../../../../scripting/lua/lua/lgc.c \
                   ../../../../scripting/lua/lua/linit.c \
                   ../../../../scripting/lua/lua/liolib.c \
                   ../../../../scripting/lua/lua/llex.c \
                   ../../../../scripting/lua/lua/lmathlib.c \
                   ../../../../scripting/lua/lua/lmem.c \
                   ../../../../scripting/lua/lua/loadlib.c \
                   ../../../../scripting/lua/lua/lobject.c \
                   ../../../../scripting/lua/lua/lopcodes.c \
                   ../../../../scripting/lua/lua/loslib.c \
                   ../../../../scripting/lua/lua/lparser.c \
                   ../../../../scripting/lua/lua/lstate.c \
                   ../../../../scripting/lua/lua/lstring.c \
                   ../../../../scripting/lua/lua/lstrlib.c \
                   ../../../../scripting/lua/lua/ltable.c \
                   ../../../../scripting/lua/lua/ltablib.c \
                   ../../../../scripting/lua/lua/ltm.c \
                   ../../../../scripting/lua/lua/lundump.c \
                   ../../../../scripting/lua/lua/lvm.c \
                   ../../../../scripting/lua/lua/lzio.c \
                   ../../../../scripting/lua/lua/print.c \
                   ../../../../scripting/lua/cocos2dx_support/CCLuaEngine.cpp \
                   ../../../../scripting/lua/cocos2dx_support/CCLuaBridge.cpp \
                   ../../../../scripting/lua/cocos2dx_support/CCLuaStack.cpp \
                   ../../../../scripting/lua/cocos2dx_support/CCLuaValue.cpp \
                   ../../../../scripting/lua/cocos2dx_support/Cocos2dxLuaLoader.cpp \
                   ../../../../scripting/lua/cocos2dx_support/LuaCocos2d.cpp \
                   ../../../../scripting/lua/cocos2dx_support/CCBProxy.cpp \
                   ../../../../scripting/lua/cocos2dx_support/Lua_extensions_CCB.cpp \
                   ../../../../scripting/lua/cocos2dx_support/Lua_web_socket.cpp \
                   ../../../../scripting/lua/cocos2dx_support/lua_cocos2dx_extensions_manual.cpp \
                   ../../../../scripting/lua/tolua/tolua_event.c \
                   ../../../../scripting/lua/tolua/tolua_is.c \
                   ../../../../scripting/lua/tolua/tolua_map.c \
                   ../../../../scripting/lua/tolua/tolua_push.c \
                   ../../../../scripting/lua/tolua/tolua_to.c \
                   ../../../../scripting/lua/cocos2dx_support/tolua_fix.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
                    $(LOCAL_PATH)/../../Classes/Scene \
                    $(LOCAL_PATH)/../../Classes/Scene/FightScene \
                    $(LOCAL_PATH)/../../Classes/Scene/homeScene \
                    $(LOCAL_PATH)/../../Classes/Engine \
                    $(LOCAL_PATH)/../../Classes/Engine/EngineSystem \
                    $(LOCAL_PATH)/../../Classes/Engine/FSMSystem \
                    $(LOCAL_PATH)/../../Classes/Engine/GUISystem \
                    $(LOCAL_PATH)/../../Classes/Engine/MsgSystem \
                    $(LOCAL_PATH)/../../Classes/Engine/NetSystem \
                    $(LOCAL_PATH)/../../Classes/Engine/ScriptSystem \
                    $(LOCAL_PATH)/../../Classes/Logic \
                    $(LOCAL_PATH)/../../Classes/Logic/State \
                    $(LOCAL_PATH)/../../Classes/Sprites \
                    $(LOCAL_PATH)/../../Classes/Public \
                    $(LOCAL_PATH)/../../../../scripting/lua \
                    $(LOCAL_PATH)/../../../../scripting/lua/lua \
                    $(LOCAL_PATH)/../../../../external/extra/native \
                    $(LOCAL_PATH)/../../../../external/extra \
                    $(LOCAL_PATH)/../../../../scripting/lua/luajit/include \
                    $(LOCAL_PATH)/../../../../scripting/lua/tolua \
                    $(LOCAL_PATH)/../../../../scripting/lua/cocos2dx_support

LOCAL_WHOLE_STATIC_LIBRARIES += libSingleEye
LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_lua_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
