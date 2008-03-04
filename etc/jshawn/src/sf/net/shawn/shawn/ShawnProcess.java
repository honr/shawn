/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
package sf.net.shawn.shawn;

import java.io.BufferedInputStream;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.LinkedList;

import org.apache.log4j.Category;

import sf.net.shawn.tools.Logging;
import sf.net.shawn.tools.TimeDiff;
import sf.net.shawn.tools.Tools;

public class ShawnProcess {

	private Category log = Logging.get(this);

	private Process shawn = null;

	private PrintWriter stdinStream = null;

	private BufferedInputStream stdoutStream = null;

	private BufferedInputStream stderrStream = null;

	private String name = "<noname>";

	private boolean taskRunning = false;

	private int historySize = 0;

	private LinkedList<ShawnResult> currentResults = new LinkedList<ShawnResult>();

	private LinkedList<String> commandHistory = new LinkedList<String>();

	// --------------------------------------------------------------------------------
	/**
	 * @param executablePath
	 * @throws IOException
	 */
	ShawnProcess(String executablePath, String workingDir) throws IOException {
		executablePath += " -jshawn";
		log.debug("Starting shawn: " + executablePath + ", workdir: " + workingDir);

		Runtime rt = Runtime.getRuntime();

		shawn = rt.exec(executablePath, null, new File(workingDir));

		if (!isRunning()) {
			log.error("Shawn executable not found.");
			return;
		}

		stdinStream = new PrintWriter(new OutputStreamWriter(shawn.getOutputStream()));

		new Thread() {
			public void run() {
				int available = 0;
				byte[] buf = new byte[256];

				//
				stderrStream = new BufferedInputStream(shawn.getErrorStream());
				while (true) {
					try {
						// Check if the error stream has bytes available and sleep if not
						available = stderrStream.available();
						if (available == 0) {
							Tools.sleep(100);
							continue;
						}

						available = stderrStream.read(buf);

						// Check if we have reached the end of the stream
						if (available == -1) {
							log.warn("End of std-error stream reached");
							break;
						} else if (available > 0) {
							System.err.write(buf, 0, available);
							System.err.flush();
						}

					} catch (IOException e) {
						log.debug("Shawn terminated: Error while reading from std-error stream: " + e, e);
						notifyTaskDone();
						killProcess();
						break;
					}
				}
			}

		}.start();

		stdoutStream = new BufferedInputStream(shawn.getInputStream());
		new StreamParser(this, stdoutStream, System.out);
	}

	// --------------------------------------------------------------------------------
	/**
	 * @param name
	 * @param value
	 */
	public void setGlobalVariable(String name, String value) {
		String sc = name + "=" + value;
		log.debug("Setting global variable: " + sc);
		stdinStream.println(sc);
		stdinStream.flush();
	}

	// --------------------------------------------------------------------------------
	/**
	 * @param cmd
	 * @param params
	 */
	public synchronized LinkedList<ShawnResult> runCommand(String cmd) {
		return runCommand(cmd, "");
	}

	// --------------------------------------------------------------------------------
	/**
	 * @param cmd
	 * @param params
	 */
	public synchronized LinkedList<ShawnResult> runCommand(String cmd, String params) {
		taskRunning = true;
		currentResults = new LinkedList<ShawnResult>();
		String cmdLine = cmd + " " + params;

		// Save the command in the history
		commandHistory.addLast(cmdLine);
		while (commandHistory.size() > historySize)
			commandHistory.removeFirst();

		// Send the command to Shawn
		log.debug("Sending line to shawn: " + cmdLine);
		stdinStream.println(cmdLine);
		stdinStream.flush();

		// Wait for the task to finish
		while (taskRunning) {
			try {
				wait();
			} catch (InterruptedException e) {
				log.error("While waiting for task done: " + e, e);
				e.printStackTrace();
			}
		}

		log.info("Task done: " + cmd);
		return currentResults;
	}

	// --------------------------------------------------------------------------------
	/**
	 * @param res
	 */
	public synchronized void addResult(ShawnResult res) {
		if (log.isDebugEnabled())
			log.debug("New result: " + res);
		currentResults.add(res);
	}

	// --------------------------------------------------------------------------------
	/**
	 * 
	 */
	public synchronized void notifyTaskDone() {
		log.debug("Task done. Number of results: " + currentResults.size());
		taskRunning = false;
		notifyAll();
	}

	// --------------------------------------------------------------------------------
	/**
	 * @return
	 */
	public boolean isRunning() {
		try {
			shawn.exitValue();
			return false;
		} catch (IllegalThreadStateException itse) {
			return true;
		}
	}

	// --------------------------------------------------------------------------------
	/**
	 *
	 */
	public void killProcess() {
		log.info("Destroying shawn process.");
		try {

			// Close the stdin stream of shawn to indicate eof
			if (stdinStream != null) {
				this.stdinStream.flush();
				this.stdinStream.close();
			}

			// Wait for termination (exit value available)
			TimeDiff timer = new TimeDiff();
			while (timer.s() < 5) {
				try {
					log.debug("Exit value of shawn: " + shawn.exitValue());
					break;
				} catch (IllegalThreadStateException e) {
					log.debug("Shawn process is still alive");
					Tools.sleep(100);
				}
			}

			// Close the out streams
			try {
				if (stderrStream != null)
					stderrStream.close();
			} catch (IOException e) {
				log.error("Error while closing stream: " + e, e);
			}

			try {
				if (stdoutStream != null)
					stdoutStream.close();
			} catch (IOException e) {
				log.error("Error while closing stream: " + e, e);
			}

			// Now forcibly terminate shawn if still running
			shawn.destroy();

			log.debug("Terminated shawn process, isRunning[" + isRunning() + "]");
		} catch (Throwable t) {
			log.error("Error while destroying shawn process: " + t, t);
		}
	}

	// --------------------------------------------------------------------------------
	/**
	 * @throws IOException
	 */
	public synchronized void saveHistory(String filename) throws IOException {
		File f = new File(filename);
		BufferedWriter w = new BufferedWriter(new FileWriter(f));

		w.write("// This file was written by JShawn (http://swarmnet.de/shawn) on "
				+ new SimpleDateFormat("yyyy.MM.dd G 'at' HH:mm:ss z").format(new Date()) + "\n\n");
		w.write("// The current max. command history size is " + getHistorySize() + " and " + commandHistory.size() + " commands were stored.");
		w.write("// If you miss commands in this file, use public void setHistorySize(int historySize) to increase the history size.");
		w.write("// The default history size is " + historySize);

		for (String cmd : commandHistory)
			w.write(cmd + "\n");

		w.flush();
		w.close();
	}

	// --------------------------------------------------------------------------------
	/**
	 * @throws IOException
	 */
	public synchronized void clearHistory() {
		commandHistory.clear();
	}

	// --------------------------------------------------------------------------------
	/**
	 * 
	 */
	public String getInstanceName() {
		return name;
	}

	// --------------------------------------------------------------------------------
	/**
	 * 
	 */
	public void setInstanceName(String name) {
		this.name = name;
	}

	// --------------------------------------------------------------------------------
	/**
	 * 
	 */
	public int getHistorySize() {
		return historySize;
	}

	// --------------------------------------------------------------------------------
	/**
	 * 
	 */
	public void setHistorySize(int historySize) {
		this.historySize = historySize;
	}

}
