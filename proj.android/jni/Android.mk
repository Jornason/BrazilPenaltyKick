LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libsqlite
LOCAL_SRC_FILES := ../thirdparty/libsqlite.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/cocos2dx/util.cpp \
                   ../../Classes/db/SqliteHandler.cpp \
                   ../../Classes/global.cpp \
                   ../../Classes/HelloWorldScene.cpp \
                   ../../Classes/MainMenuLayer.cpp \
                   ../../Classes/PlayScene.cpp \
                   ../../Classes/ScoreScene.cpp \
                   ../../Classes/util/util.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					/Users/beckheng/Downloads/sqlite-amalgamation-3080301

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += spritebuilder_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += libsqlite

include $(BUILD_SHARED_LIBRARY)

$(call import-module,2d)
$(call import-module,audio/android)
$(call import-module,Box2D)
$(call import-module,editor-support/spritebuilder)

