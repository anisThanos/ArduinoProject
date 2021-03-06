package seiot.modulo_lab_4_1;

import java.sql.*;
import java.util.Calendar;

import seiot.modulo_lab_2_2.msg.jssc.*;

public class Controller /* implements ActionListener */ {

	SerialCommChannel channel;
	private final Connection conn;

	public Controller(String port, LogView logger, Connection conn) throws Exception {
		channel = new SerialCommChannel(port, 9600);
		this.conn = conn;
		new MonitoringAgent(channel, /* view, */logger, conn).start();

		/* attesa necessaria per fare in modo che Arduino completi il reboot */

		System.out.println("Waiting Arduino for rebooting...");
		Thread.sleep(4000);
		System.out.println("Ready.");

	}

	public void sendHumidity(float value) {
		try {
			channel.sendMsg(String.valueOf(value));
			Calendar calendar = Calendar.getInstance();
			java.sql.Date startDate = new java.sql.Date(calendar.getTime().getTime());
			Timestamp timestamp = new Timestamp(startDate.getTime());
			System.out.println(timestamp);

			// the mysql insert statement
			String query = " insert into dati (type, value, tempo)" + " values ( ?, ?, ?)";

			// create the mysql insert preparedstatement
			PreparedStatement preparedStmt = conn.prepareStatement(query);
			preparedStmt.setString(1, "H");
			preparedStmt.setString(2, String.valueOf(value));
			preparedStmt.setTimestamp(3, timestamp);

			// execute the preparedstatement
			preparedStmt.execute();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}
