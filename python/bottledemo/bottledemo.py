#!/usr/bin/env python
# -*- coding: utf-8 -*-

from bottle import route, run, template, get, post, static_file, request
import argparse

@get('/js/<filename:re:.*\.js>')
def javascripts(filename):
    return static_file(filename, root='js')

@get('/css/<filename:re:.*\.css>')
def stylesheets(filename):
    return static_file(filename, root='css')

@get('/img/<filename:re:.*\.(jpg|png|gif|ico)>')
def images(filename):
    return static_file(filename, root='img')

@get('/fonts/<filename:re:.*\.(eot|ttf|woff|svg)>')
def fonts(filename):
    return static_file(filename, root='fonts')

@get('/<filename:re:.*\.(html|htm|xml)>')
def htmls(filename):
    return static_file(filename, root='.')

@get('/favicon.ico')
def favicon():
    return static_file('favicon.ico', root='.')

@route('/hello/<name>')
def index(name):
    return template('<b>Hello {{name}}</b>!', name=name)

# default template dirs are '.' and 'views'
@route('/hello2/<name>')
def index(name):
    return template('hello.html', name=name)

@get('/login') # or @route('/login')
def login():
    return '''
        <form action="/login" method="post">
            Username: <input name="username" type="text" />
            Password: <input name="password" type="password" />
            <input value="Login" type="submit" />
        </form>
    '''

@post('/login') # or @route('/login', method='POST')
def do_login():
    username = request.forms.get('username')
    password = request.forms.get('password')
    return "<p>Your login information was correct.</p>"
    #if check_login(username, password):
    #    return "<p>Your login information was correct.</p>"
    #else:
    #    return "<p>Login failed.</p>"

@route('/json')
def returnjson(user):
    from bottle import response
    from json import dumps
    rv = [
        {
            'title': '写代码',
            'time': '2014-06-11 11:23:23',
            },
        {
            'title': '继续写代码',
            'time': '2014-06-11 11:22:23',
            },
        ]
    response.content_type = 'application/json'
    return dumps(rv)

parser = argparse.ArgumentParser(description='bottle web server for rotate test.')
parser.add_argument("-s", action='store', dest='host', default='localhost', help='bind server ip or host name')
parser.add_argument("-p", action='store', dest='port', default='8080', help='bind port')
args = parser.parse_args()
run(host=args.host, port=args.port, reloader=True)
