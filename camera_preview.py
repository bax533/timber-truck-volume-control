from pyzed import sl
import cv2

zed = sl.Camera()

# Set configuration parameters
init_params = sl.InitParameters()
init_params.camera_resolution = sl.RESOLUTION.HD1080
init_params.camera_fps = 30

# Open the camera
err = zed.open(init_params)
if err != sl.ERROR_CODE.SUCCESS:
    exit(-1)

image = sl.Mat(zed.get_camera_information().camera_resolution.width, zed.get_camera_information().camera_resolution.height, sl.MAT_TYPE.U8_C4)
key = 0
it = 0

while(zed.grab() == sl.ERROR_CODE.SUCCESS and key != 27):
  # A new image is available if grab() returns SUCCESS
    zed.retrieve_image(image, sl.VIEW.LEFT) # Retrieve the left image
    image_ocv = image.get_data()
    # Display the left image from the numpy array
    cv2.imshow("Image", image_ocv)
    key = cv2.waitKey(0)
    print(key)

    if(key == 115):
        cv2.imwrite("saved_photos/{}_zed.png".format(it), image_ocv)

    it += 1
