package com.cwm.logging;

import java.util.Hashtable;

import com.cwm.util.PropertyResourceBundle;

/*
 * An implementation of the Logger class which performs no function and is simply used as place holder 
 * in case no other logger has been specified.
 */
public class EmptyLogger implements Logger {

	private boolean loggingON = false;

	public void initialise(PropertyResourceBundle messageCatalog,
			String loggerID, String resourceName) {
	}

	public void setResourceName(String logContext) {
	}

	public boolean isLoggable(int level) {
		return false;
	}

	public void severe(String sourceClass, String sourceMethod, String msg) {
		if (loggingON) {
			System.out.println(System.currentTimeMillis() + ":" + sourceClass
					+ ":" + sourceMethod + ":" + msg);
		}
	}

	public void severe(String sourceClass, String sourceMethod, String msg,
			Object[] inserts) {
		if (loggingON) {
			System.out.println(System.currentTimeMillis() + ":" + sourceClass
					+ ":" + sourceMethod + ":" + msg);
		}
	}

	public void severe(String sourceClass, String sourceMethod, String msg,
			Object[] inserts, Throwable thrown) {
		if (loggingON) {
			System.out.println(System.currentTimeMillis() + ":" + sourceClass
					+ ":" + sourceMethod + ":" + msg);
		}
	}

	public void warning(String sourceClass, String sourceMethod, String msg) {
		if (loggingON) {
			System.out.println(System.currentTimeMillis() + ":" + sourceClass
					+ ":" + sourceMethod + ":" + msg);
		}
	}

	public void warning(String sourceClass, String sourceMethod, String msg,
			Object[] inserts) {
		if (loggingON) {
			System.out.println(System.currentTimeMillis() + ":" + sourceClass
					+ ":" + sourceMethod + ":" + msg);
		}
	}

	public void warning(String sourceClass, String sourceMethod, String msg,
			Object[] inserts, Throwable thrown) {
		if (loggingON) {
			System.out.println(System.currentTimeMillis() + ":" + sourceClass
					+ ":" + sourceMethod + ":" + msg);
		}
	}

	public void info(String sourceClass, String sourceMethod, String msg) {
		if (loggingON) {
			System.out.println(System.currentTimeMillis() + ":" + sourceClass
					+ ":" + sourceMethod + ":" + msg);
		}
	}

	public void info(String sourceClass, String sourceMethod, String msg,
			Object[] inserts) {
		if (loggingON) {
			System.out.println(System.currentTimeMillis() + ":" + sourceClass
					+ ":" + sourceMethod + ":" + msg);
		}
	}

	public void info(String sourceClass, String sourceMethod, String msg,
			Object[] inserts, Throwable thrown) {
		if (loggingON) {
			System.out.println(System.currentTimeMillis() + ":" + sourceClass
					+ ":" + sourceMethod + ":" + msg);
		}
	}

	public void config(String sourceClass, String sourceMethod, String msg) {
		if (loggingON) {
			System.out.println(System.currentTimeMillis() + ":" + sourceClass
					+ ":" + sourceMethod + ":" + msg);
		}
	}

	public void config(String sourceClass, String sourceMethod, String msg,
			Object[] inserts) {
		if (loggingON) {
			System.out.println(System.currentTimeMillis() + ":" + sourceClass
					+ ":" + sourceMethod + ":" + msg);
		}
	}

	public void config(String sourceClass, String sourceMethod, String msg,
			Object[] inserts, Throwable thrown) {
		if (loggingON) {
			System.out.println(System.currentTimeMillis() + ":" + sourceClass
					+ ":" + sourceMethod + ":" + msg);
		}
	}

	public void fine(String sourceClass, String sourceMethod, String msg) {
		if (loggingON) {
			System.out.println(System.currentTimeMillis() + ":" + sourceClass
					+ ":" + sourceMethod + ":" + msg);
		}
	}

	public void fine(String sourceClass, String sourceMethod, String msg,
			Object[] inserts) {
		if (loggingON) {
			System.out.println(System.currentTimeMillis() + ":" + sourceClass
					+ ":" + sourceMethod + ":" + msg);
		}
	}

	public void fine(String sourceClass, String sourceMethod, String msg,
			Object[] inserts, Throwable ex) {
		if (loggingON) {
			System.out.println(System.currentTimeMillis() + ":" + sourceClass
					+ ":" + sourceMethod + ":" + msg);
		}
	}

	public void finer(String sourceClass, String sourceMethod, String msg) {
		if (loggingON) {
			System.out.println(System.currentTimeMillis() + ":" + sourceClass
					+ ":" + sourceMethod + ":" + msg);
		}
	}

	public void finer(String sourceClass, String sourceMethod, String msg,
			Object[] inserts) {
		if (loggingON) {
			System.out.println(System.currentTimeMillis() + ":" + sourceClass
					+ ":" + sourceMethod + ":" + msg);
		}
	}

	public void finer(String sourceClass, String sourceMethod, String msg,
			Object[] inserts, Throwable ex) {
		if (loggingON) {
			System.out.println(System.currentTimeMillis() + ":" + sourceClass
					+ ":" + sourceMethod + ":" + msg);
		}
	}

	public void finest(String sourceClass, String sourceMethod, String msg) {
		if (loggingON) {
			System.out.println(System.currentTimeMillis() + ":" + sourceClass
					+ ":" + sourceMethod + ":" + msg);
		}
	}

	public void finest(String sourceClass, String sourceMethod, String msg,
			Object[] inserts) {
		if (loggingON) {
			System.out.println(System.currentTimeMillis() + ":" + sourceClass
					+ ":" + sourceMethod + ":" + msg);
		}
	}

	public void finest(String sourceClass, String sourceMethod, String msg,
			Object[] inserts, Throwable ex) {
		if (loggingON) {
			System.out.println(System.currentTimeMillis() + ":" + sourceClass
					+ ":" + sourceMethod + ":" + msg);
		}
	}

	public void log(int level, String sourceClass, String sourceMethod,
			String msg, Object[] inserts, Throwable thrown) {
		if (loggingON) {
			System.out.println(System.currentTimeMillis() + ":" + sourceClass
					+ ":" + sourceMethod + ":" + msg);
		}
	}

	public void trace(int level, String sourceClass, String sourceMethod,
			String msg, Object[] inserts, Throwable ex) {
		if (loggingON) {
			System.out.println(System.currentTimeMillis() + ":" + sourceClass
					+ ":" + sourceMethod + ":" + msg);
		}
	}

	public String formatMessage(String msg, Object[] inserts) {
		return null;
	}

	public void dumpTrace() {
	}

	public Hashtable getProperties() {
		return null;
	}

}
