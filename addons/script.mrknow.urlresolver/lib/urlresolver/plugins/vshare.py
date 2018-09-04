"""
    urlresolver XBMC Addon
    Copyright (C) 2011 t0mm0

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""

#todo: get quality check

import re
from urlresolver import common
from urlresolver.resolver import UrlResolver, ResolverError

class VshareResolver(UrlResolver):
    name = "vshare"
    domains = ['vshare.io']
    pattern = '(?://|\.)(vshare\.io)/\w?/(\w+)'

    def __init__(self):
        self.net = common.Net()

    def get_media_url(self, host, media_id):
        web_url = self.get_url(host, media_id)
        link = self.net.http_GET(web_url).content
        if link.find('404 - Error') >= 0:
            raise ResolverError('The requested video was not found.')

        #video_link = str(re.compile("url[: ]*'(.+?)'").findall(link))
        video_link = re.compile('"url":"(.*?)",.* ?"label":"(.*?)"', re.DOTALL).findall(link)
        print ("V",video_link,len(video_link))
        if len(video_link) > 0:
            print("tutaj",video_link[0][0].replace('\\',''))
            return str(video_link[0][0]).replace('\\','')
        else:
            raise ResolverError('No playable video found.')

    def get_url(self, host, media_id):
        #return 'http://vshare.io/v/%s/width-620/height-280/' % media_id
        #print media_id
        return 'http://vshare.io/v/%s/width-650/height-430' % media_id
