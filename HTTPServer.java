//package httpserverjava;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class HTTPServer {
    public static void main(String[] args) {
	int port;
	ServerSocket serverSocket;
	try{
	    port = Integer.parseInt(args[0]);
	} catch(Exception e) {
	    System.out.println("port=8080(default)");
	    port = 8080;
	}

	try{
	    serverSocket = new ServerSocket(port);
	    System.out.println("server listening port:"+serverSocket.getLocalPort());
	    while(true){
		Socket socket = serverSocket.accept();
		System.out.println("new tcp connect, address is:"+socket.getInetAddress()+":"+socket.getPort());
		try{
		    service(socket);
		} catch (Exception e) {
		    e.printStackTrace();
		}
	    }
	} catch (IOException e) {
	    e.printStackTrace();
	}
    }

    public static void service(Socket socket) throws Exception {
	InputStream socketIn = socket.getInputStream();
	Thread.sleep(500);
	int size = socketIn.available();
	byte[] buffer = new byte[size];
	socketIn.read(buffer);
	String request = new String(buffer);
	System.out.println(request);
	if (request.indexOf("\r\n") == -1) {
	    System.out.println("request.indexOfCRLF: "+request.indexOf("\r\n")+", return");
	    return;
	}

	String firstLineOfRequest = request.substring(0, request.indexOf("\r\n"));
	System.out.println("fist line of http request:" + firstLineOfRequest);
	String[] parts = firstLineOfRequest.split(" ");
	String uri = parts[1];
	String contentType;
	if (uri.indexOf("html") != -1 || uri.indexOf("htm") != -1)
	    contentType = "text/html";
	else if (uri.indexOf("jpg") != -1 || uri.indexOf("jpeg") != -1)
	    contentType = "image/jpeg";
	else if (uri.indexOf("gif") != -1)
	    contentType = "image/gif";
	else
	    contentType = "application/octet-stream";

	String responseFirstLine = "HTTP/1.1 200 OK\r\n";
	String responseHeader = "Content-type:" + contentType + "\r\n\r\n";
	InputStream in = HTTPServer.class.getResourceAsStream(uri);

	OutputStream socketOut = socket.getOutputStream();
	socketOut.write(responseFirstLine.getBytes());
	socketOut.write(responseHeader.getBytes());

	int len = 0;
	buffer = new byte[128];
	while((len = in.read(buffer)) != -1)
	    socketOut.write(buffer, 0, len);

	Thread.sleep(1000);
	socket.close();
    }
}
