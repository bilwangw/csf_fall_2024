Bill Wang
Ryan Wu

We both collaborated on the code. We generally work together to pair program.

For our implementation, we ensured that the table data structure was synchronized across the
server and clients. The client itself keeps track of an internal list of which tables it has
modified, which is used when committing or rolling back changes.

To synchronize the tables, we made a vector of table pointers in our server. This way, whenever
a client needs to modify a table, it will directly edit the tables stored inside the server.
We created helper functions in the server to handle creating/deleting, locking/unlocking, and
finding whether or not a table exists.