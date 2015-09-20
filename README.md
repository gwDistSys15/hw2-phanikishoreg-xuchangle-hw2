# Distributed Conversion System

###### Assignment 2, CSCI 6461 Distributed Systems by Prof. Tim Wood.

###Student Authors:
	1. Phani Kishore Gadepalli (phanikishoreg@gwu.edu)
	2. Changle Xu (xuchangle@gwu.edu)

###Conversion System:
	1. Feet <-> Inches - C program
	2. Inches <-> Centimetres - Java program
	3. More Servers!!
		1. Centimetres <-> Meters - Python program
		2. Bananas <-> Inches - Python program
		3. Bananas <-> Pounds of Bananas - C program

	4. Proxy Server (Configurable)

#### Feet <-> Inches (conv_server_1)

This is a C program. 
To Compile, 
```
$gcc -o conv_server conv_server.c
```
To run, use the following syntax:
```
$./conv_server <portno>
```
where "./conv_server" is the executable name and <portno> is the port number for this server to listen to.
```
ex:
$./conv_server 9999
```

To convert between Feet and Inches, use the following input format: (same as described by Professor)
```
ft in <value><\n>
or
in ft <value><\n>

Ex:
ft in 100
```
Response: Response from the server will be a float value.
```
(for above example)
1200.000000
```

#### Inches <-> Centimetres (conv_server_2)

This is a Java program. 
To compile,
```
$javac ConvServer.java
```
To run use the following syntax:
```
$java ConvServer <portno>
```
where "ConvServer" is the Server class and <portno> is the port number for this server to listen to.
```
Ex:
$java ConvServer 9998
```
To convert between Inches and Centimetres, use the following input format: (same as described by Professor)
```
cm in <value><\n>
or
in cm <value><\n>

Ex:
in cm 1200
```
Response: Response from the server will be a float value. 
```
(for above example)
3048.00
```
#### More Servers!!! (Just to make multi-level going.. :-( ) 

**Meters <-> Centimetres** (conv_server_3)

This is a Python script. To run, use the following syntax:
```
$python convServer.py <portno>
```
where "convServer.py" is the python script name and <portno> is the port number for this server to listen to.
```
Ex:
$python convServer.py 9997
```
To convert between Meters and Centimetres, use the following input format: (same as described by Professor)
```
cm m <value><\n>
or
m cm <value><\n>

Ex:
cm m 1200
```
Response: Response from the server will be a float value. 
```
(for above example)
12.0
```
**Bananas <-> Inches** (conv_server_4)

This is a Python script. To run, use the following syntax:
```
$python convServer.py <portno>
```
where "convServer.py" is the python script name and <portno> is the port number for this server to listen to.
```
Ex:
$python convServer.py 9996
```
To convert between Bananas and Inches, use the following input format: (same as described by Professor)
```
b in <value><\n>
or
in b <value><\n>

Ex:
b in 100
```
Response: Response from the server will be a float value. 
```
(for above example)
500.0
```
*(using conversion value: 1 Banana = 5.0 Inches)*

**Bananas <-> Pounds of Bananas** (conv_server_5)

This is a C Program. 
To compile,
```
$gcc -o conv_server conv_server.c
```
To run, use the following syntax:
```
$./conv_server <portno>
```
where "conv_server" is the executable name and <portno> is the port number for this server to listen to.
```
Ex:
$./conv_server 9995
```
To convert between Bananas and Pounds of Bananas, use the following input format: (same as described by Professor)
```
b lbs <value><\n>
or
lbs b <value><\n>

Ex:
lbs b 100
```
Response: Response from the server will be a float value. 
```
(for above example)
300.000000
```
*(using conversion value: 1 Banana = 3.0 pounds)*


## Proxy Server

Proxy server is a Java based server and at this point supports conversions individually and multi-level conversions using a conversion table which is configurable at Compile-time (if you've the source code) or at Run-time (using a config file).

To compile: 
```
$javac ProxyServer.java
```

#### Starting Proxy server
There are 2 ways to start our proxy server

**1**. using static conversion data: Conversion server list and conversion table is set within the initConversionsStatic() method and it can be extended before compilation. See the next section (How to configure) for more details.

To run:
```
$java ProxyServer <portno>
```
where ProxyServer is the class name and portno is the port number for the server to listen to.
```
Ex: 
$java ProxyServer 7777
```
**2**. using file conversion data: Conversion server list and conversion table can be fed to the Proxy server through a file. This is parsed from initConversionsFile() method. See the next section (How to configure) for details.

To run:
```
$java ProxyServer <portno> [config file]
```
where ProxyServer is the class name and portno is the port number for the server to listen to and "config file" is a file that contains conversion data in the format mentioned in next section.
```
$java ProxyServer 7777 convdata.conf
```

#### Conversions (How To Configure)

There are two ways this proxy server can be configured to convert from any unit to any other unit.

#####1. At Compile time. You'll need source code for this kind of configuration.

Inside initConversionsStatic() function, we populate two Maps,
	1. Conversion server list
	2. Conversion table

You can add any conversion server you've got and any conversion chain that you'd like.

Code snippet of existing static conversion table in the Program.

```
// Add list of conversion servers here.
convServers.put("in->ft", new Server("localhost", 9999));
convServers.put("ft->in", new Server("localhost", 9999));
convServers.put("in->cm", new Server("localhost", 9998));
convServers.put("cm->in", new Server("localhost", 9998));
convServers.put("cm->m", null); // not supported yet
convServers.put("m->cm", null); // not supported yet
 
// Keeping directed routing makes it lot easier..!!
// Support one level conversions using conversion server list.
convTable.put("ft->in", "ft->in");
convTable.put("in->ft", "in->ft");
convTable.put("in->cm", "in->cm");
convTable.put("cm->in", "cm->in");
 
// Support two level conversions using conversion server list.
convTable.put("ft->cm", "ft->in,in->cm");
convTable.put("cm->ft", "cm->in,in->ft");
 
// Support three or more level conversions using conversion server list.
convTable.put("ft->m", "ft->in,in->cm,cm->m");
convTable.put("m->ft", "m->cm,cm->in,in->ft");
```

#####2. At Run-time. You'll either add to existing config file or write a new one. 

Config file format:
```
=CONVERSION_SERVER_LIST
in->ft,localhost:9999
ft->in,localhost:9999
in->cm,localhost:9998
cm->in,localhost:9998
=END_LIST
=CONVERSION_TABLE
ft->in:ft->in 
in->ft:in->ft
in->cm:in->cm
cm->in:cm->in
ft->cm:ft->in,in->cm 
cm->ft:cm->in,in->ft
ft->m:ft->in,in->cm,cm->m
m->ft:m->cm,cm->in,in->ft
=END_TABLE
```

Tags required: "=CONVERSION_SERVER_LIST" for starting the Conversion server listing and "=END_LIST" for ending it. After start tag line and before end tag line, every line should be of this format:
```
<conversion-type>,<server ip/hostname>:<server portnum>
```

Similarly, "=CONVERSION_TABLE" followed by "=END_TABLE". After start tag line and before end tag line, every line should be of the below format:
```
<conversion-type>:<sub-conv-type1>,<sub-conv-type2>,<sub-conv-type3>,..,<sub-conv-typen>
```
**Very Important:** (In writing Conversion server list or Conversion table - for Static or Dynamic)
* If a line starts with '#', it is regarded as comment and ignored.
* Conversion table: conversion-type and sub-conv-type combination should be in both directions if you need support for x to y and y to x conversions like in the above example.
* Conversion server list: conversion-type, if you've a server that supports for example, in->ft and ft->in, please add two entries respectively.
* Please make sure there are no extra space characters either at the beginning or ending or anywhere else in the file. Proxy server is not mature enough to trim the spaces or ignore junk in the file. It may terminate abruptly if such a character is encountered.
* You'll need to add conversions in both the directions and in the order of conversions to take place.
* Please do not add multiple servers to do the same conversion, behavior is not tested and it is definitely not supported to connect to the second or later servers if first connection fails.  (Failure handling - BIG NO)

#### Conversions (Currently configured using own servers)

* following is the conversion table supported by default by our Proxy server (run statically or with provided config file (convdata.conf))
```
in<->ft: 1 level using conv_server_1 at port 9999
cm<->in: 1 level using conv_server_2 at port 9998
m<->cm: 1 level using conv_server 3 at port 9997
b<->in: 1 level using conv_server_4 at port 9996
b<->lbs: 1 level using conv_server_5 at port 9995
```
Following multi-level conversions supported using above mentioned conversion servers. 
```
ft->cm:ft->in,in->cm
cm->ft:cm->in,in->ft
m->in:m->cm,cm->in
in->m:in->cm,cm->m
b->cm:b->in,in->cm
cm->b:cm->in,in->b
b->ft:b->in,in->ft
ft->b:ft->in,in->b
lbs->in:lbs->b,b->in
in->lbs:in->b,b->lbs
ft->m:ft->in,in->cm,cm->m
m->ft:m->cm,cm->in,in->ft
b->m:b->in,in->cm,cm->m
m->b:m->cm,cm->in,in->b
lbs->ft:lbs->b,b->in,in->ft
ft->lbs:ft->in,in->b,b->lbs
lbs->cm:lbs->b,b->in,in->cm
cm->lbs:cm->in,in->b,b->lbs
lbs->m:lbs->b,b->in,in->cm,cm->m
m->lbs:m->cm,cm->in,in->b,b->lbs
```
**For these conversions to work, the required conversion server has to be up and listening to the ports mentioned in the above list or in the config data.**

#### Response to clients

If the conversion was successful, a float value is returned to the Client. 
And if the conversion failed for some reason, as there is no defined standard, each Server could do different things.
* one can just close the socket without responding.
* one can return a empty string followed by \n and close the socket.
* one can return a string message with error like, "Not a number\n", "Invalid Input\n", "Incorrect conversion\n", etc.

Things could get messier if Proxy server just forwards the response from one Conversion server to next server. The failure at one server just continues through all conversion levels.

**All the servers developed by us, if they fail to convert, they close the socket without sending a response.**

#### Limitations
* Conversion servers or the Proxy server are not tested fully, so there are issues.
   If you encounter any, please report to the authors to fix.

   Especially the Exceptions related to Connections/Parsing conversion data from file etc are not handled.

#### Distributed System Characteristics
1. **Openness**: All you need to do is build a Conversion table and it works for you. 
2. **Scalability**: Only problem is, it fetches only at startup!
3. **Heterogeneity**: It doesn't care which language or which operating system or what area your other server belongs to. Idea is, as long as we follow a Protocol of messages, we are fine.
4. *Security, Failure handling, Concurency*: Not considered.  *Qos*: I guess if you're satisfied, we've done a good job with QoS. :P
5. *Transparency*: Well, as long as the servers are up and running, you can access it from anywhere. But once you're connected and you've same IP and port number and are not in a isolated network, I think it should work.

