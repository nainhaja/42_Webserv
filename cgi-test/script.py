import cgi, cgitb, time

form = cgi.FieldStorage() 

fname = form.getvalue('first_name')
lname = form.getvalue('last_name')

print ("Content-type:text/html\r\n\r\n")
print ("<html>")
print ("<head>")
print ("<title>Hello - Second CGI Program</title>")
print ("</head>")
print ("<body>")
print ("<h2>Hello %s %s</h2>" % (fname, lname))
print ("</body>")
print ("</html>")