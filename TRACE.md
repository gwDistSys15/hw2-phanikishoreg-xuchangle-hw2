### TRACE 1 (Using remote servers)

The best trace that we got connecting to remote servers so far is something like this:

Question: How many grams of potassium does a 1000 cm length of bananas (laid out end to end ) contain??
```
Accepted connection from client: /127.0.0.1


Request: 1000 cm = ? g
Step 1/3 using localhost:9998 => 1000 cm = 393.7008 in
Step 2/3 using 54.152.180.217:5554 => 393.7008 in = 65.6168 b
Step 3/3 using 54.152.180.217:5555 => 65.6168 b = 30.971129599999998 g
Response: 1000 cm = 30.9711295999999998 g


done.
```
* 1000 cm of bananas was converted to 393.7 inches of bananas using our own server.
* 393.7008 inches of bananas is converted to 65.6168 bananas using other team's server.
* 65.6168 bananas converted to 30.97.. grams of potassium using other team's server.

This trace was generated using two remote servers at Step 2 and 3 by Yi Zhou (and team) and one local server developed by us.
Unfortunately this is the best we could do with the number of servers being online at that momemt.

**Tried with other servers that were posted on Piazza but we couldn't connect to them, probably because they were taken down at the time we tried.**

### TRACE 2 (Only own servers)

With all the local servers that we have, following was 4level conversion done:
Question: If we have to lay bananas end to end for 1 meter, how many pounds of bananas do we need?
```
Accepted connection from client: /54.173.235.102


Request: 1 m = ? lbs
Step 1/4 using localhost:9997 => 1 m = 100.0 cm
Step 2/4 using localhost:9998 => 100.0 cm = 39.37008 in
Step 3/4 using localhost:9996 => 39.37008 in = 7.874016 b
Step 4/4 using localhost:9995 => 7.874016 b = 2.624672 lbs
Response: 1 m = 2.624672 lbs


done.
```

* 1m of bananas converted to 2.62.. lbs using 4 levels of our own conversion servers.