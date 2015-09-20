/******************************************************************************
 *
 *  CS 6421 - Simple Conversation for in<->cm
 *  Compilation:  javac ConvServer.java
 *  Execution:    java ConvServer port
 *
 *  % java ConvServer portnum
 *
 *  Students: Phani, Changle
 *
 ******************************************************************************/

import java.net.Socket;
import java.net.ServerSocket;
import java.io.IOException;
import java.io.PrintWriter;
import java.net.UnknownHostException;
import java.io.BufferedReader;
import java.io.InputStreamReader;

public class ConvServer {

	/* 
	 * Converts to/from Inches/Cemtimeters
	 * from - from conversion
	 * to - to conversion
	 * inp - input value
	 *
	 * return - output value. -1.0 for failure
	 */
	public static float convert(String from, String to, float inp) {
		
		if(from.equals("in") && to.equals("cm")) { // inches to centi
			return inp * 2.54f;
		} else if(from.equals("cm") && to.equals("in")) { //centi to inches
			return inp / 2.54f;
		}
		return -1.0f; //invalid conv..
	}

	public static void process (Socket clientSocket) throws IOException {
		// open up IO streams
		BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
		PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);

		/* Write a welcome message to the client */
		//out.println("Welcome, you're connected to Inches (in) <-> Centimeters (cm) Conversion server in Java");
		out.println("in<->cm conversion in Java");

		/* read and print the client's request */
		// readLine() blocks until the server receives a new line from client
		String userInput;
		if ((userInput = in.readLine()) == null) {
			System.out.println("Error reading message");
			out.close();
			in.close();
			clientSocket.close();
			
			return; //exit from this client..
		}

		String[] tokens = userInput.split(" ");
		String fromConv = "", toConv = "";
		float inputValue = 0.0f;
		int noOfTokens = 0;
		for(String s:tokens){
			if(noOfTokens == 0) { //input conv token..
				if(!(s.equals("in") || s.equals("cm"))) {
					System.out.println("Invalid input : " + userInput);
					out.close();
					in.close();
					clientSocket.close();

					return; //exit from this client..
				}
				fromConv = s;
				noOfTokens ++;
			} else if(noOfTokens == 1) { //output conv token..
				if(!(s.equals("in") || s.equals("cm"))) {
					System.out.println("Invalid input : " + userInput);
					out.close();
					in.close();
					clientSocket.close();

					return; //exit from this client..
				}
				toConv = s;
				noOfTokens ++;
			} else if(noOfTokens == 2) { //input value token..
				try {
					inputValue = Float.parseFloat(s);
				} catch (NumberFormatException nfe) {
					System.out.println("NumberFormatException caught.." + nfe.getMessage());
					out.close();
					in.close();
					clientSocket.close();

					return; //exit from this client..
				}
				noOfTokens ++;
			} else { //invalid number of tokens..
				System.out.println("Invalid input here: " + userInput);
				out.close();
				in.close();
				clientSocket.close();

				return; //exit from this client..
			}
		}		

		//convert inches <-> centi..
		float outputValue = convert(fromConv, toConv, inputValue);
		if(outputValue < 0.0f) {
			System.out.println("Conversion failed..");
			out.close();
			in.close();
			clientSocket.close();

			return; //exit from this client..
		}

		System.out.println(inputValue + " " + fromConv + " = " + outputValue + " " + toConv);
		out.println(outputValue); //write result to socket..

		// close IO streams, then socket
		out.close();
		in.close();
		clientSocket.close();
	}

	public static void main(String[] args) throws Exception {

		ServerSocket serverSocket = null;
		try {
			//check if argument length is invalid
			if(args.length != 1) {
				System.err.println("Usage: java ConvServer port");
				System.exit(-1);
			}
			// create socket
			int port = Integer.parseInt(args[0]);
			serverSocket = new ServerSocket(port);
			System.err.println("Started server on port " + port);
		} catch (Exception e) { //any exception, same treatment..
			System.err.println("Caught exception: " + e.getMessage());
			System.exit(-1);
		}

		// wait for connections, and process
		try {
			while(true) {
				// a "blocking" call which waits until a connection is requested
				Socket clientSocket = serverSocket.accept();
				System.err.println("\nAccepted connection from client - " + clientSocket.getInetAddress());
				try {
					process(clientSocket);
				} catch (Exception e) {
					//If one client process fails with any Exception, don't terminate the server!!
					System.err.println("Caught exception: " + e.getMessage());
					//lets hope, we can serve others..
					continue;
				}
			}

		} catch (IOException ioe) {
			System.err.println("Caught IOException: " + ioe.getMessage());
		} catch (Exception e) {
			//something bad happened.. have to exit..
			System.err.println("Caught Exception: " + e.getMessage());
		}
		System.exit(-1); //should never come here..!!
	}
}
