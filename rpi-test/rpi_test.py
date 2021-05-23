from samsam_api import Samsam_api
from gpio_test import Sensor

def main():
	email = "samsam-uos@gmail.com"
	password = "33wing"
	samsam_api = Samsam_api()
	samsam_api.login(email, password)
	padbox = samsam_api.get_padbox(55)
	print(padbox)

	sensor = Sensor()
	dist = sensor.get_dist()
	print("dist:", dist)
	print("padbox:", dist//3)
	samsam_api.patch_padbox_state(55, 33, dist//3, 33)

	padbox = samsam_api.get_padbox(55)
	print(padbox)



if __name__ == '__main__':
	main()

