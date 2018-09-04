# -*- coding: utf-8 -*-
"""
openload.io urlresolver plugin
Copyright (C) 2015 tknorris

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
"""
import hashlib
import os
import json
from lib import pyaes
from urlresolver import common
from urlresolver.resolver import UrlResolver, ResolverError

API_BASE_URL = 'https://api.openload.co/1'
INFO_URL = API_BASE_URL + '/streaming/info'
GET_URL = API_BASE_URL + '/streaming/get?file={media_id}'
OL_PATH = os.path.join(common.plugins_path, 'ol_gmu.py')

class OpenLoadResolver(UrlResolver):
    name = "openload"
    domains = ["openload.io", "openload.co"]
    pattern = '(?://|\.)(openload\.(?:io|co))/(?:embed|f)/([0-9a-zA-Z-_]+)'

    def __init__(self):
        self.net = common.Net()

    @common.cache.cache_method(cache_limit=1)
    def get_ol_code(self):
        try:
            ol_source = self.get_setting('url')
            ol_key = self.get_setting('key')
            if ol_source and ol_key:
                headers = self.net.http_HEAD(ol_source).get_headers(as_dict=True)
                common.log_utils.log(headers)
                old_etag = self.get_setting('etag')
                new_etag = headers.get('Etag', '')
                old_len = self.__old_length()
                new_len = int(headers.get('Content-Length', 0))
                if old_etag != new_etag or old_len != new_len:
                    common.log_utils.log('Updating ol_gmu: |%s|%s|%s|%s|' % (old_etag, new_etag, old_len, new_len))
                    self.set_setting('etag', new_etag)
                    new_py = self.net.http_GET(ol_source).content
                    if new_py:
                        new_py = self.__decrypt(new_py, ol_key)
                        if new_py:
                            with open(OL_PATH, 'w') as f:
                                f.write(new_py)
                            common.kodi.notify('OpenLoad Resolver Auto-Updated')
                else:
                    common.log_utils.log('Reusing existing ol_gmu.py: |%s|%s|%s|%s|' % (old_etag, new_etag, old_len, new_len))
        except Exception as e:
            common.log_utils.log_warning('Exception during OpenLoad code retrieve: %s' % (e))
            
    def __decrypt(self, cipher_text, key):
        if cipher_text:
            try:
                scraper_key = hashlib.sha256(key).digest()
                IV = '\0' * 16
                decrypter = pyaes.Decrypter(pyaes.AESModeOfOperationCBC(scraper_key, IV))
                plain_text = decrypter.feed(cipher_text)
                plain_text += decrypter.feed()
                if 'get_media_url' not in plain_text:
                    plain_text = ''
            except Exception as e:
                common.log_utils.log_warning('Exception during OpenLoad Decrypt: %s' % (e))
                plain_text = ''
        else:
            plain_text = ''

        return plain_text
    
    def __old_length(self):
        try:
            with open(OL_PATH, 'r') as f:
                old_py = f.read()
            old_len = len(old_py)
        except:
            old_len = -1
        return old_len

    def get_media_url(self, host, media_id):
        try:
            if self.get_setting('auto_update') == 'true':
                self.get_ol_code()
                
            with open(OL_PATH, 'r') as f:
                py_data = f.read()
                
            common.log_utils.log('ol_gmu hash: %s' % (hashlib.md5(py_data).hexdigest()))
            import ol_gmu
            web_url = self.get_url(host, media_id)
            return ol_gmu.get_media_url(web_url)
        except Exception as e:
            common.log_utils.log_debug('Exception during openload resolve parse: %s' % (e))
            try:
                video_url = self.__check_auth(media_id)
                if not video_url:
                    video_url = self.__auth_ip(media_id)
            except ResolverError:
                raise
            
            if video_url:
                return video_url
            else:
                raise ResolverError('No OpenLoad Authorization')

    def get_url(self, host, media_id):
        return 'http://openload.co/embed/%s' % (media_id)

    def __auth_ip(self, media_id):
        js_data = self.__get_json(INFO_URL)
        pair_url = js_data.get('result', {}).get('auth_url', '')
        if pair_url:
            pair_url = pair_url.replace('\/', '/')
            header = 'OpenLoad Stream Authorization'
            line1 = 'To play this video, authorization is required'
            line2 = 'Visit the link below to authorize the devices on your network:'
            line3 = '[B][COLOR blue]%s[/COLOR][/B] then click "Pair"' % (pair_url)
            with common.kodi.CountdownDialog(header, line1, line2, line3) as cd:
                return cd.start(self.__check_auth, [media_id])
        
    def __check_auth(self, media_id):
        try:
            js_data = self.__get_json(GET_URL.format(media_id=media_id))
        except ResolverError as e:
            status, msg = e
            if status == 403:
                return
            else:
                raise ResolverError(msg)
        
        return js_data.get('result', {}).get('url')
    
    def __get_json(self, url):
        result = self.net.http_GET(url).content
        js_result = json.loads(result)
        common.log_utils.log_debug(js_result)
        if js_result['status'] != 200:
            raise ResolverError(js_result['status'], js_result['msg'])
        return js_result

    @classmethod
    def get_settings_xml(cls):
        xml = super(cls, cls).get_settings_xml()
        xml.append('<setting id="%s_auto_update" type="bool" label="Automatically update resolver" default="true"/>' % (cls.__name__))
        xml.append('<setting id="%s_url" type="text" label="    Auto-Update Url" default="" visible="eq(-1,true)"/>' % (cls.__name__))
        xml.append('<setting id="%s_key" type="text" label="    Decryption Key" default="" option="hidden" visible="eq(-2,true)"/>' % (cls.__name__))
        xml.append('<setting id="%s_etag" type="text" default="" visible="false"/>' % (cls.__name__))
        return xml
