#include <mysql.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	int err;
	
	// Create a connection to the database
	conn = mysql_init(NULL);
	if (conn == NULL) {
		printf("Error connecting to database: %s\n", mysql_error(conn));
		return 1;
	}
	
	conn = mysql_real_connect(conn, "localhost", "root", "mysql", "Uno", 0, NULL, 0);	
	if (conn == NULL) {
		printf("Error connecting to database: %s\n", mysql_error(conn));
		return 1;
	}
	
	
	err = mysql_query(conn,"SELECT * FROM Game");
	if(err!=0){
		printf("Error trying to consult the database %s\n", mysql_error(conn));
		exit(1);
	}
	
	//Ask the username
	char username[30];
	printf("Write the username of the player whose game with most movements you want to see:\n");
	scanf("%s", username);
	
	// Execute the query
	char query[256];
	sprintf(query, "SELECT g.GameID, g.GameDate, g.NumOfMoves"
			"FROM Game g "
			"JOIN Turns t ON g.GameID = t.GameID "
			"JOIN Player p ON t.PlayerID = p.PlayerID "
			"WHERE p.Username = '%s' "
			"GROUP BY g.GameID "
			"ORDER BY g.NumOfMoves DESC "
			"LIMIT 1", username);
	
	// Get the result
	res = mysql_store_result(conn);
	if (!res) {
		printf("Error getting result: %s\n", mysql_error(conn));
		return 1;
	}
	int printed = 0;
	// Print the result
	while ((row = mysql_fetch_row(res))) {
		if(!printed){
			printf("The game with most movements played by %s has the identifier %d ," 
				   "and it was held on the %s, with a total of %s moves.", username, atoi(row[0]), row[1], row[4]);
			printed = 1;
		}		
	}
	
	// Free the result
	mysql_free_result(res);
	
	// Close the connection
	mysql_close(conn);
	
	return 0;
}
