# Create your views here.

from django.shortcuts import render_to_response
from django.http import HttpResponseRedirect

def submitcsrf(request):
    print 'request info: ' request.REQUEST['user'], request.REQUEST['repayment']
    return HttpResponseRedirect('/thanks.html')
