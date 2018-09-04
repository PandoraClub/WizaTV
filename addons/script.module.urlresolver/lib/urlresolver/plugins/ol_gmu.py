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
import urllib
import re
import urllib2
from lib.aa_decoder import AADecoder
from lib.jjdecode import JJDecoder
from HTMLParser import HTMLParser
from urlresolver import common
from urlresolver.resolver import ResolverError

net = common.Net()
SIZE_LIMIT = 32 * 1024 * 1024

def caesar_shift(s, shift=13):
    s2 = ''
    for c in s:
        if c.isalpha():
            limit = 90 if c <= 'Z' else 122
            new_code = ord(c) + shift
            if new_code > limit:
                new_code -= 26
            s2 += chr(new_code)
        else:
            s2 += c
    return s2

def unpack(html):
    strings = re.findall('{\s*var\s+a\s*=\s*"([^"]+)', html)
    shifts = re.findall('\)\);}\((\d+)\)', html)
    for s, shift in zip(strings, shifts):
        s = caesar_shift(s, int(shift))
        s = urllib.unquote(s)
        for i, replace in enumerate(['j', '_', '__', '___']):
            s = s.replace(str(i), replace)
        html += '<script>%s</script>' % (s)
    return html

def get_media_url(url):
    try:
        HTTP_HEADER = {
            'User-Agent': 'Mozilla/5.0 (Windows NT 6.1; WOW64; rv:39.0) Gecko/20100101 Firefox/39.0',
            'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
            'Accept-Charset': 'ISO-8859-1,utf-8;q=0.7,*;q=0.3',
            'Accept-Encoding': 'none',
            'Accept-Language': 'en-US,en;q=0.8',
            'Referer': url}  # 'Connection': 'keep-alive'

        html = net.http_GET(url, headers=HTTP_HEADER).content
        try: html = html.encode('utf-8')
        except: pass
        html = unpack(html)
        match = re.search('''>([^<]+)</span>\s*<span\s+id="streamurl"''', html, re.DOTALL | re.IGNORECASE)
        if not match:
            raise ResolverError('Stream Url Not Found. Deleted?')
        
        hiddenurl = HTMLParser().unescape(match.group(1))
        
        decodes = []
        for match in re.finditer('<script[^>]*>(.*?)</script>', html, re.DOTALL):
            encoded = match.group(1)
            match = re.search("(ﾟωﾟﾉ.*?\('_'\);)", encoded, re.DOTALL)
            if match:
                decodes.append(AADecoder(match.group(1)).decode())
                
            match = re.search('(.=~\[\].*\(\);)', encoded, re.DOTALL)
            if match:
                decodes.append(JJDecoder(match.group(1)).decode())
            
        if not decodes:
            raise ResolverError('No Encoded Section Found. Deleted?')
        
        magic_number = 0
        for decode in decodes:
            match = re.search('charCodeAt\(\d+\)\s*\+\s*(\d+)\)', decode, re.DOTALL | re.I)
            if match:
                magic_number = match.group(1)
                break

        s = []
        for idx, i in enumerate(hiddenurl):
            j = ord(i)
            if (j >= 33 & j <= 126):
                j = 33 + ((j + 14) % 94)
                
            if idx == len(hiddenurl) - 1:
                j += int(magic_number)
            s.append(chr(j))
        res = ''.join(s)
        
        videoUrl = 'https://openload.co/stream/{0}?mime=true'.format(res)
        dtext = videoUrl.replace('https', 'http')
        headers = {'User-Agent': HTTP_HEADER['User-Agent']}
        req = urllib2.Request(dtext, None, headers)
        res = urllib2.urlopen(req)
        videourl = res.geturl()
        if int(res.headers['Content-Length']) < SIZE_LIMIT:
            raise ResolverError('Openload.co resolve failed. Pigeons?')
        res.close()
        
        return videourl
    except Exception as e:
        common.log_utils.log_debug('Exception during openload resolve parse: %s' % e)
        raise

    raise ResolverError('Unable to resolve openload.io link. Filelink not found.')
