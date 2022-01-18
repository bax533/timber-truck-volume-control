from pyzed import sl
import cv2

zed = sl.Camera()

# Set configuration parameters
init_params = sl.InitParameters()
init_params.camera_fps = 30
init_params.depth_mode = sl.DEPTH_MODE.ULTRA # Use ULTRA depth mode
init_params.coordinate_units = sl.UNIT.CENTIMETER
# Open the camera
err = zed.open(init_params)
if err != sl.ERROR_CODE.SUCCESS:
    exit(-1)

image = sl.Mat(zed.get_camera_information().camera_resolution.width, zed.get_camera_information().camera_resolution.height, sl.MAT_TYPE.U8_C4)
depth_map = sl.Mat()
key = 0
it = 0

while(zed.grab() == sl.ERROR_CODE.SUCCESS and key != 27):
  # A new image is available if grab() returns SUCCESS
    zed.retrieve_image(image, sl.VIEW.LEFT) # Retrieve the left image
    zed.retrieve_measure(depth_map, sl.MEASURE.DEPTH)
    image_ocv = image.get_data()
    depth_ocv = depth_map.get_data()
    # Display the left image from the numpy array
    cv2.imshow("Image", image_ocv)
    key = cv2.waitKey(0)
    print(key)

    if(key == 115):
        cv2.imwrite("saved_photos/{}_zed.png".format(it), image_ocv)
        cv2.imwrite("saved_photos/{}_zed_depth.png".format(it), depth_ocv)
        depth_file = open("saved_photos/{}_depth_file.txt".format(it), "w")
        for row in depth_ocv:
            for x in row:
                depth_file.write(str(x)+"\n")

    it += 1
