import cgi, cgitb
import sys

form = cgi.FieldStorage()

original_stdout = sys.stdout

prenom = form.getvalue('Prenom')
nom = form.getvalue('Nom')
age = form.getvalue('Age')

with open("python_debug.txt", 'w') as f:
	sys.stdout = f
	print("Got form: {}".format(form))
	sys.stdout = original_stdout

# print("""
# Content-type:text/html\r\n\r\n\
# <html>
# <body>
# <p>Got keys: {}</p>
# </body>
# </html>
# """.format(', '.join(list(form.keys()))))

# print("Content-type:text/html\r\n\r\n")
# print("<html>")
# print("<head>")
# print("<title>Webserv</title>")
# print("</head>")
# print("<body>")
# print("<h2>Salut {} {}, {} ans.".format(prenom, nom, age))
# print("<p>Got keys:")
# print("</body>")
# print("</html>")