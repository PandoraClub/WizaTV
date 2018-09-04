'''
    urlresolver XBMC Addon
    Copyright (C) 2016 Gujal

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
'''

import re
from lib import jsunpack
from urlresolver import common
from urlresolver.resolver import UrlResolver, ResolverError

class MegaMP4Resolver(UrlResolver):
    name = "megamp4.net"
    domains = ["megamp4.net"]
    pattern = '(?://|\.)(megamp4\.net)/(?:embed-|emb\.html\?|)([0-9a-zA-Z]+)'

    def __init__(self):
        self.net = common.Net()

    def get_media_url(self, host, media_id):
        web_url = self.get_url(host, media_id)

        html = self.net.http_GET(web_url).content

        if 'was deleted' in html:
            raise ResolverError('File Removed')


        js_data = re.findall('(eval\(function.*?)</script>', html.replace('\n', ''))

        for i in js_data:
            try: html += jsunpack.unpack(i)
            except: pass


        link = re.search('file:"(.*?)",', html)
        if link:
            return link.group(1)
            
        raise ResolverError('Unable to find megamp4 video')

    def get_url(self, host, media_id):
        return 'http://megamp4.net/embed-%s.html' % (media_id)
