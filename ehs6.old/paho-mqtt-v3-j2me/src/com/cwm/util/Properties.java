package com.cwm.util;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.Hashtable;

/**
 * An simple implementation much like java.util.Properties in Java SE. It
 * enables reading an {@link InputStream} with key=value pairs (one per line).
 * You can then access values by key.
 * 
 * @author batkinson
 * 
 */
public class Properties {

	private Hashtable propTable = new Hashtable();

	/**
	 * Loads the properties object from the given {@link InputStream}.
	 * 
	 * @param in
	 * @throws IOException
	 */
	public void load(InputStream in) throws IOException {
		InputStreamReader reader = new InputStreamReader(in);
		StringBuffer lineBuf = new StringBuffer();
		char[] streamBuf = new char[25];
		int count = -1;

		while ((count = reader.read(streamBuf)) >= 0) {
			int end = 0, start = end;
			for (; end < count; end++) { // Split buffer into lines if multiple
				if (streamBuf[end] == '\r') {
					lineBuf.append(streamBuf, start, end - start);
					processLine(lineBuf.toString());
					lineBuf = new StringBuffer();
					if (end + 1 < count && streamBuf[end + 1] == '\n')
						end++;
					start = end + 1;
				} else if (streamBuf[end] == '\n') {
					lineBuf.append(streamBuf, start, end - start);
					processLine(lineBuf.toString());
					lineBuf = new StringBuffer();
					start = end + 1;
				}
			}
			// If scan left contents in buffer, add it
			if (start < count && start != end)
				lineBuf.append(streamBuf, start, end - start);
		}

		// Process last line
		if (lineBuf.length() > 0)
			processLine(lineBuf.toString());
	}

	/**
	 * Processes a line as a key=value pair and inserts into the table. If the
	 * value is a comment or doesn't look like a key=value pair, it skips over
	 * it.
	 * 
	 * @param line
	 */
	private void processLine(String line) {
		if (line.startsWith("#"))
			return; // Skip comments
		int equalsIndex = line.indexOf('=');

		// Skip lines that don't look like property definitions
		if (equalsIndex == -1)
			return;

		String key = line.substring(0, equalsIndex).trim();
		String value = equalsIndex + 1 < line.length() ? line.substring(
				equalsIndex + 1).trim() : "";
		propTable.put(key, value);
	}

	/**
	 * Adds the key=value pair to the table.
	 * 
	 * @param key
	 * @param value
	 */
	public void setProperty(String key, String value) {
		propTable.put(key, value);
	}

	/**
	 * Fetches the value for the given key.
	 * 
	 * @param key
	 * @return the value or null if one doesn't exist for key
	 */
	public String getProperty(String key) {
		return (String) propTable.get(key);
	}

	/**
	 * Fetches the value for the given key, or the default value if no value is
	 * found.
	 * 
	 * @param key
	 * @param defaultValue
	 * @return the valur for key or the default value.
	 */
	public String getProperty(String key, String defaultValue) {
		String value = (String) propTable.get(key);
		return value == null ? defaultValue : value;
	}
}
