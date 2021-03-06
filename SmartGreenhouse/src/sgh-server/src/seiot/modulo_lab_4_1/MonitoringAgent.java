package seiot.modulo_lab_4_1;

import java.sql.*;
import java.util.Calendar;
import seiot.modulo_lab_2_2.msg.jssc.*;

public class MonitoringAgent extends Thread {

	SerialCommChannel channel;
	LogView logger;
	Connection conn;

	static final String IRR_PREFIX = "ir:";
	static final String LOG_PREFIX = "lo:";
	static final String ERROR_PREFIX = "er:";

	public MonitoringAgent(SerialCommChannel channel, LogView log, Connection conn) throws Exception {
		this.logger = log;
		this.channel = channel;
		this.conn = conn;
	}

	public void run() {
		while (true) {
			try {
				String msg = channel.receiveMsg();
				Calendar calendar = Calendar.getInstance();
				java.sql.Date startDate = new java.sql.Date(calendar.getTime().getTime());
				Timestamp timestamp = new Timestamp(startDate.getTime());

				// the mysql insert statement
				String query = " insert into dati (type, value, tempo)" + " values ( ?, ?, ?)";
				if (msg.startsWith(IRR_PREFIX)) {
					String time = msg.substring(IRR_PREFIX.length());
					try {
						// create the mysql insert preparedstatement
						PreparedStatement preparedStmt = conn.prepareStatement(query);
						preparedStmt.setString(1, "I");
						preparedStmt.setString(2, time);
						preparedStmt.setTimestamp(3, timestamp);

						// execute the preparedstatement
						preparedStmt.execute();

					} catch (Exception e) {
						System.out.println(e);
					}
				} else if (msg.startsWith(LOG_PREFIX)) {
					this.logger.log(msg.substring(LOG_PREFIX.length()));
				} else if (msg.startsWith(ERROR_PREFIX)) {
					String segnalation = msg.substring(ERROR_PREFIX.length());
					try {
						// create the mysql insert preparedstatement
						PreparedStatement preparedStmt = conn.prepareStatement(query);
						preparedStmt.setString(1, "E");
						preparedStmt.setString(2, segnalation);
						preparedStmt.setTimestamp(3, timestamp);

						// execute the preparedstatement
						preparedStmt.execute();

					} catch (Exception e) {
						System.out.println(e);
					}
				}
			} catch (Exception ex) {
				ex.printStackTrace();
			}
		}
	}

}
