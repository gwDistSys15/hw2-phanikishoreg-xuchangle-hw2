# Distributed Conversion System

###### Assignment 2, CSCI 6461 Distributed Systems by Prof. Tim Wood.

###Student Authors:
	1. Phani Kishore Gadepalli (phanikishoreg@gwu.edu)
	2. Changle Xu (xuchangle@gwu.edu)

###Conversion System:
	1. Feet <-> Inches - C program
	2. Inches <-> Centimetres - Java program
	3. Centimetres <-> Meters - Python program

	4. Proxy Server (Configurable)

#### Feet <-> Inches

This is a C program. To run, use the following syntax:
```
$./conv_server <portno>
```
where "./conv_server" is the executable name and <portno> is the port number for this server to listen to.
```
ex:
$./conv_server 9999
```

To convert from Feet to Inches, use the following input format: (same as described by Professor)
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
1200.00
```

#### Inches <-> Centimetres

This is a Java program. To run use the following syntax:
```
$java ConvServer <portno>
```
where "ConvServer" is the Server class and <portno> is the port number for this server to listen to.
```
Ex:
$java ConvServer 9998
```
To convert from Inches to Centimetres, use the following input format: (same as described by Professor)
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

#### Meters <-> Centimetres

This is a Python script. To run, use the following syntax:
```
$python convServer.py <portno>
```
where "convServer.py" is the python script name and <portno> is the port number for this server to listen to.
```
Ex:
$python convServer.py 9997
```
To convert from Meters to Centimetres, use the following input format: (same as described by Professor)
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

## Proxy Server

Proxy server is a Java based server and at this point supports conversions individually and multi-level conversions using a conversion table which is configurable at Compile-time (if you've the source code) or at Run-time (using a config file).

#### Starting Proxy server
There are 2 ways to start our proxy server

**1**. using static conversion data: Conversion server list and conversion table is set within the initConversionsStatic() method and it can be extended before compilation. See the next section (How to configure) for more details.

To compile: 
```
$javac ProxyServer.java
```
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

To compile:
```
$javac ProxyServer.java
```
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
* Conversion table: conversion-type and sub-conv-type combination should be in both directions if you need support for x to y and y to x conversions like in the above example.
* Conversion server list: conversion-type, if you've a server that supports for example, in->ft and ft->in, please add two entries respectively.
* Please make sure there are no extra space characters either at the beginning or ending or anywhere else in the file. Proxy server is not mature enough to trim the spaces or ignore junk in the file. It may terminate abruptly if such a character is encountered.
* You'll need to add conversions in both the directions and in the order of conversions to take place.
* Please do not add multiple servers to do the same conversion, behavior is not tested and it is definitely not supported to connect to the second or later servers if first connection fails.  (Failure handling - BIG NO)

#### Conversions (Currently configured *using local and remote servers)

1. ft <-> in : using local server
2. ft <-> cm : using local server
3. cm <-> m : using local server
4. ft <-> cm : using 2 levels
5. ft <-> m : using 3 levels
6. in <-> m : using 2 levels
(* we are not getting remote servers in the above categories. :-( )
4. b <-> g : using remote server by XXXX
5. b <-> in : using remote server by XXXX
6. g <-> cm : g <-> b, b <-> in, in <-> cm
7. g <-> m : g <-> b, b <-> in, in <-> cm, cm <-> m
8. g <-> ft : g <-> b, b <-> in, in <-> ft
(* one set of remote servers)


#### Response to clients

If the conversion was successful, a float value is returned to the Client. 
And if the conversion failed for some reason, as there is no defined standard, each Server could do different things.
* one can just close the socket without responding.
* one can return a empty string followed by \n and close the socket.
* one can return a string message with error like, "Not a number\n", "Invalid Input\n", "Incorrect conversion\n", etc.

Things could get messier if Proxy server just forwards the response from one Conversion server to next server. The failure at one server just continues through all conversion levels.

#### Limitations
* Conversion servers or the Proxy server are not tested fully, so there are issues.
   If you encounter any, please report to the authors to fix.

   Especially the Exceptions related to Connections/Parsing conversion data from file etc are not handled.

#### Distributed System Characteristics
1. **Openness**: All you need to do is build a Conversion table and it works for you. Only problem is, it fetches only at startup!
2. **Heterogeneity**: It doesn't care which language or which operating system or what area your other server belongs to. Idea is, as long as we follow a Protocol of messages, we are fine.
3. *Security, Failure handling, Concurency*: Lol.  I guess if you're satisfied, we've done a good job with QoS. :P
4. *Transparency*: Well, as long as the servers are up and running, you can access it from anywhere. But once you're connected and you've same IP and port number and are not in a isolated network, I think it should work.
