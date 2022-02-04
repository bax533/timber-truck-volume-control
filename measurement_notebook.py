import numpy as np

Ds = [0.41, 0.41, 0.38, 0.35, 0.31, 0.58, 0.45]
Rs = [d/2 for d in Ds]

circleCenterPoints = [(541,300)*2,(472, 228)*2, (398, 346)*2, (464, 294)*2, (499,339)*2, (366, 266)*2, (387, 167)*2]
distsToAbuts = [2.58, 2.78, 2.72, 3.09, 3.06, 2.94, 2.47] 

Ls = [5.53, 5.62, 5.35, 5.08, 5.65, 5.0, 5.12]

Areas = [r*r*3.14 for r in Rs]
for a in Areas:
    print(a, "CIRCLE AREA")

Vs = []
for it in range(7):
    Vs.append(Areas[it]*Ls[it])

print(sum(Vs), "VOLUME")

px_to_m = 0.006

Rs_prog = [32*px_to_m, 35*px_to_m, 35*px_to_m, 27*px_to_m, 26*px_to_m, 47*px_to_m, 45*px_to_m]
Areas_prog = [r*r*3.14 for r in Rs_prog]

Vs_prog = []

print("errors of radiuses")
errs = []
for i in range(7): 
    errs.append(abs(Rs[i]-Rs_prog[i])/Rs[i] * 100.0)
    print(abs(Rs[i]-Rs_prog[i])/Rs[i] * 100.0, "%", Ds[i], i)

print("mean error", sum(errs)/len(errs))
print("SEM = ", np.std(Rs_prog)*np.sqrt(1-np.var(Rs)/np.var(Rs_prog)))

print("")

print("errors of areas")
errs_area = []
for i in range(7):
    errs_area.append(abs(Areas[i]-Areas_prog[i])/Areas[i] * 100.0)
    Vs_prog.append(Areas_prog[i]*Ls[i])
    print(abs(Areas[i]-Areas_prog[i])/Areas[i] * 100.0, "%")

print("mean error", sum(errs_area)/len(errs_area))
print("vars ", np.var(Areas_prog)/np.var(Areas), )
print("SEM =", np.std(Areas_prog)*np.sqrt(1 - np.var(Areas)/np.var(Areas_prog)))

print(sum(Vs_prog), "VOLUME PROG WITH ORIGINAL LENGTHS")




########################### LENGTH RECONSTRUCTION

imgWidth = 2208
imgHeigth = 1242
fov_y = 64.0
fov_x = 96.0

#alpha
def horAngleForPos(x):
    center = imgWidth/2
    horizontalFovPerPixel = fov_y/imgHeigth

    angle = abs(center - x)*horizontalFovPerPixel*3.1415926535/180.0
    return angle

def verAngleForPixel(y):
    center = imgHeigth/2
    verticalFovPerPixel = fov_x/imgWidth
    angle = abs(center - y)*verticalFovPerPixel * 3.1415926535/180.0
    return angle
D = 8.4
#print([angleForPos(pos[0]) for pos in circleCenterPoints])
alphas = [horAngleForPos(pos[0]) for pos in circleCenterPoints]
betas = [verAngleForPixel(pos[1]) for pos in circleCenterPoints]
dz = [distsToAbuts[i]*np.sqrt( ((np.sin(alphas[i])**2 - 1)/(np.sin(alphas[i])**2 * np.sin(betas[i])**2 - 1))
    - ( (np.sin(alphas[i])**2 - 1)/(np.sin(alphas[i])**2 - 1/np.sin(betas[i])) ) ) for i in range(7)]

onlyHorizontalSubs = [distsToAbuts[i]*np.sin(1.57 - horAngleForPos(circleCenterPoints[i][0])) for i in range(7)]

calculated_lengths = [D - dz[i] for i in range(7)]
print("LENGTHS")
print(calculated_lengths)

errs_length = []
for i in range(7):
    e = abs(Ls[i] - calculated_lengths[i])/Ls[i] * 100.0
    errs_length.append(e)

Vs_prog_2 = []
for i in range(7):
    Vs_prog_2.append(Areas_prog[i]*calculated_lengths[i])


print(sum(Vs_prog_2), "vs", sum(Vs))

errs_vol = []
for i in range(7):
    errs_vol.append(abs(Vs[i]-Vs_prog_2[i])/Vs[i] * 100.0)

print("mean error of volume calc =", sum(errs_vol)/len(errs_vol))

#print("SEM =", np.std(calculated_lengths)*np.sqrt(1 - np.var(Ls)/np.var(calculated_lengths)))

