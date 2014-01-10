#ifndef GCS_H
#define GCS_H

#define GCS_BOOL_SIZE 100
/*
Grand Central Storage - Fancy name for configuration information.
It is NOT well defined, there are only slots! Use at your own risk.
Will be protected by futexes in the future, as well as having more well defined structure. In documentation form only
*/
class GCS { //Note, singleton class
	private:
		static GCS *gcs_instance;
		GCS();
		bool kernel_boolean_table[GCS_BOOL_SIZE];
	public:
		static GCS *instance();
		bool getBool(int index);
};

#endif