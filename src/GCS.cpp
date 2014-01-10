#include <gcs.h>

GCS *GCS::gcs_instance = 0;

GCS::GCS()
{
	instance(); //Ensures there is an instance
}

GCS *GCS::instance() {
	if (!gcs_instance)
		gcs_instance = new GCS();
	return gcs_instance;
}

bool GCS::getBool(int index)
{
	return kernel_boolean_table[index];
}
void GCS::writeBool(int index,bool value)
{
	kernel_boolean_table[index]=value;
}