Test task #1 for C++, given me in EPAM on interview.

Task:

Client-Server application on C++ that works with two classes:
 - Student
 - Teacher

Both class has the same fields:
 - Name
 - Gender
 - Age

Each class has its own fields:
 - Student:
 	- Group
	- Average grade

 - Teacher:
 	- Chair
	- Academic Status

Client will create 2-3 objects of each cless, serialize them into XLM file and then send the file by TCP to the server.
Server should receive this XML, de-serializa it and print the content as human-readeble, for example:

- Student Alexandrov Ivan, male, 17 years old, group "ABC-41", average grade is 4.1.
- Teacher Ivanova Alexandra, female, 71 years old, chair "Applied Mathmatics", academic status is professor.

You can use any source to find information and examples by TCP client/server, XML parsing/serialization.
Recomended platform for development is : libxml2, boost.
Both client and server could be as console applications.
Recomended development time: 4-6 hours.

Please pay attension to:
Stability of server to inconsisten data that comming from client (invalid or hudge XML file).
Stability of server to servicing of a bin number of connections.
