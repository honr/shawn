/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
package sf.net.shawn.shawn;

import java.util.Iterator;
import java.util.Vector;

import org.apache.log4j.Category;

import sf.net.shawn.EnvironmentConfig;
import sf.net.shawn.tools.Logging;

public class ShawnProcessFactory {
	private static Category log = Logging.get(ShawnProcessFactory.class);

	private static Vector<ShawnProcess> shawns = new Vector<ShawnProcess>();

	private static EnvironmentConfig defaultValues = null;

	public static ShawnProcess create() throws Exception {
		log.debug("Creating shawn instance with default values");
		ShawnProcess s = new ShawnProcess(defaultValues.getShawnExecutable(), defaultValues.getWorkDir());
		shawns.add(s);
		return s;
	}

	public static void destroy() {

		log.debug("Destroying " + shawns.size() + " shawn processes.");
		for (Iterator<ShawnProcess> it = shawns.iterator(); it.hasNext();)
			it.next().killProcess();
		log.debug("Done.");
	}

	public static EnvironmentConfig getDefaultValues() {
		return defaultValues;
	}

	public static void setDefaultValues(EnvironmentConfig defaultValues) {
		ShawnProcessFactory.defaultValues = defaultValues;
	}

}

