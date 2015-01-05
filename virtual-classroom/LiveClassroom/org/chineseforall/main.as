application.onAppStart = function() {
	trace("app 'live' started");
};

// Let's define our stream list array
var stream_list = new Array();

application.onConnect = function(client, username, firstname, lastname) {
	// Accept the connection.
	application.acceptConnection(client);
	client.username = username;
	client.firstname = firstname;
	client.lastname = lastname;
	client.call("updateClientID", client.id);
	trace("connect: " + client.id + " and username = " + username + " " + firstname + " " + lastname);
};

Client.prototype.registerStream = Client_registerStream;
function Client_registerStream(client_id, name) {
	trace("register stream called: " + name);
	
	var num_clients = application.clients.length;
	for(i=0; i<num_clients; ++i) {
		if(application.clients[i].id === client_id) {
			
			// Let's add the user's stream to the stream list
			application.clients[i].streamName = name;
			stream_list.push(name);
		}
	}
	
	// Broadcast a stream list update to all clients.
	application.broadcastMsg("addStream", name);
};
