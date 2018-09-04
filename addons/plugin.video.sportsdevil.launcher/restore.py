#      Copyright (C) 2015 noobsandnerds
#
#  This Program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2, or (at your option)
#  any later version.
#
#  This Program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with XBMC; see the file COPYING.  If not, write to
#  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
#  http://www.gnu.org/copyleft/gpl.html
#

import os, re, xbmc, xbmcaddon, xbmcgui, shutil

AddonID      = 'plugin.video.sportsdevil.launcher'
ADDON        = xbmcaddon.Addon(id=AddonID)
dialog       = xbmcgui.Dialog()
ADDONS       = xbmc.translatePath(os.path.join('special://home','addons'))
packages     = os.path.join(ADDONS,'packages')
sportsdevil  = xbmc.translatePath(os.path.join(ADDONS,'plugin.video.SportsDevil'))
sd_temp      = xbmc.translatePath(os.path.join('special://home','userdata','addon_data','plugin.video.sportsdevil.launcher','old_addon'))

if os.path.exists(sd_temp):
	try:
		print "### Attempting to remove SportsDevil addon folder"
		shutil.rmtree(sportsdevil)
	except:
		print "### No existing SportsDevil addon to remove"
	try:
		shutil.copytree(sd_temp,sportsdevil)
		try:
			shutil.rmtree(packages)
		except: pass
		dialog.ok('Success','Backup SportsDevil has now been restored. Remember to disable auto-updates or remove the repo you installed the previous one from otherwise Kodi will auto update the addon again.')
	except:
		dialog.ok('Failed','Sorry there was an error trying to copy the backup to your addons folder, please try again. If that fails you can always manually copy it over via the filemanager.')
else:
	dialog.ok('No Backup Found','Sorry there doesn\'t appear to be a backup of SportsDevil on this system.')