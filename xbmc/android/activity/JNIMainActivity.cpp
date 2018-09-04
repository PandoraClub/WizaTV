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

#include "JNIMainActivity.h"

#include "android/jni/Activity.h"
#include "android/jni/Intent.h"
#include "android/jni/jutils/jutils-details.hpp"

using namespace jni;

CJNIMainActivity* CJNIMainActivity::m_appInstance(NULL);

CJNIMainActivity::CJNIMainActivity(const ANativeActivity *nativeActivity)
  : CJNIActivity(nativeActivity)
{
  m_appInstance = this;
}

CJNIMainActivity::~CJNIMainActivity()
{
  m_appInstance = NULL;
}

void CJNIMainActivity::_onNewIntent(JNIEnv *env, jobject context, jobject intent)
{
  (void)env;
  (void)context;
  if (m_appInstance)
    m_appInstance->onNewIntent(CJNIIntent(jhobject(intent)));
}

void CJNIMainActivity::_callNative(JNIEnv *env, jobject context, jlong funcAddr, jlong variantAddr)
{
  (void)env;
  (void)context;
  ((void (*)(CVariant *))funcAddr)((CVariant *)variantAddr);
}

void CJNIMainActivity::runNativeOnUiThread(void (*callback)(CVariant *), CVariant* variant)
{
  call_method<void>(m_context,
                    "runNativeOnUiThread", "(JJ)V", (jlong)callback, (jlong)variant);
}

void CJNIMainActivity::_onVolumeChanged(JNIEnv *env, jobject context, jint volume)
{
  (void)env;
  (void)context;
  if(m_appInstance)
    m_appInstance->onVolumeChanged(volume);
}

void CJNIMainActivity::_onAudioFocusChange(JNIEnv *env, jobject context, jint focusChange)
{
  (void)env;
  (void)context;
  if(m_appInstance)
    m_appInstance->onAudioFocusChange(focusChange);
}

CJNISurface CJNIMainActivity::getVideoViewSurface()
{
  return call_method<jhobject>(m_context,
                               "getVideoViewSurface", "()Landroid/view/Surface;");
}

void CJNIMainActivity::clearVideoView()
{
  call_method<void>(m_context,
                    "clearVideoView", "()V");
}

void CJNIMainActivity::setVideoViewSurfaceRect(int l, int t, int r, int b)
{
  call_method<void>(m_context,
                    "setVideoViewSurfaceRect", "(IIII)V", l, t, r, b);
}

void CJNIMainActivity::registerMediaButtonEventReceiver()
{
  call_method<void>(m_context,
                    "registerMediaButtonEventReceiver", "()V");
}

void CJNIMainActivity::unregisterMediaButtonEventReceiver()
{
  call_method<void>(m_context,
                    "unregisterMediaButtonEventReceiver", "()V");
}
