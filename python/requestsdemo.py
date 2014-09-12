import requests

def test_post_xml():
    xml = """<employees>
    <employee>
        <name>Fred Jones</name>
        <address location="home">
            <street>900 Aurora Ave.</street>
            <city>Seattle</city>
            <state>WA</state>
            <zip>98115</zip>
        </address>
        <address location="work">
            <street>2011 152nd Avenue NE</street>
            <city>Redmond</city>
            <state>WA</state>
            <zip>98052</zip>
        </address>
        <phone location="work">(425)555-5665</phone>
        <phone location="home">(206)555-5555</phone>
        <phone location="mobile">(206)555-4321</phone>
    </employee>
</employees> """
    headers = {'Content-Type': 'text/xml'} # set what your server accepts
    print requests.post('http://httpbin.org/post', data=xml, headers=headers).text


def test_cookie():
    url = 'http://httpbin.org/cookies'
    cookies = dict(cookies_are='working')
    print requests.get(url, cookies=cookies).text
