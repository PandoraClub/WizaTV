#pragma once
/*
 *      Copyright (C) 2015 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "android/jni/Activity.h"
#include "android/jni/Surface.h"

class CJNIMainActivity : public CJNIActivity
{
public:
  CJNIMainActivity(const ANativeActivity *nativeActivity);
  ~CJNIMainActivity();

  static CJNIMainActivity* GetAppInstance() { return m_appInstance; }

  static void _onNewIntent(JNIEnv *env, jobject context, jobject intent);
  static void _onVolumeChanged(JNIEnv *env, jobject context, jint volume);
  static void _onAudioFocusChange(JNIEnv *env, jobject context, jint focusChange);

  static void _callNative(JNIEnv *env, jobject context, jlong funcAddr, jlong variantAddr);
  static void runNativeOnUiThread(void (*callback)(CVariant *), CVariant *variant);
  static void registerMediaButtonEventReceiver();
  static void unregisterMediaButtonEventReceiver();

  CJNISurface getVideoViewSurface();
  void clearVideoView();
  void setVideoViewSurfaceRect(int l, int t, int r, int b);

private:
  static CJNIMainActivity *m_appInstance;

protected:
  virtual void onNewIntent(CJNIIntent intent)=0;
  virtual void onVolumeChanged(int volume)=0;
  virtual void onAudioFocusChange(int focusChange)=0;
};
