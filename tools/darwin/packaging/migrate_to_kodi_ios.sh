APP_NAME=Kodi
XBMC_HOME="/var/mobile/Library/Preferences/XBMC"
APP_HOME="/var/mobile/Library/Preferences/Kodi"


function needs_kodi_migration () {
  #check if there is an old XBMC folder to migrate
  if test -d "$XBMC_HOME" && test ! -d "$APP_HOME"
  then
    return "1";
  else
    return "0";
  fi
}

function migrate_to_kodi () {
  echo "moving settings from $XBMC_HOME to $APP_HOME"
  mv $XBMC_HOME $APP_HOME
  chown -R mobile.mobile $APP_HOME
  echo "Migration complete"
  touch $APP_HOME/.kodi_data_was_migrated
}

needs_kodi_migration
NEEDS_MIGRATION=$?
if [ "$NEEDS_MIGRATION" == "1"  ]
then
  echo "This is the first time you install Kodi and we detected that you have an old XBMC configuration. Your XBMC configuration will now be migrated to Kodi by moving the settings folder. If you ever want to go back to XBMC please backup /var/mobile/Library/Preferences/Kodi after this installation has finished and rename it to XBMC in case you downgrade to XBMC."
  migrate_to_kodi
fi
