from bottle import route, run, template, get, post, static_file
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

parser = argparse.ArgumentParser(description='bottle web server for rotate test.')
parser.add_argument("-s", action='store', dest='host', default='localhost', help='bind server ip or host name')
parser.add_argument("-p", action='store', dest='port', default='8080', help='bind port')
args = parser.parse_args()
run(host=args.host, port=args.port, reloader=True)
