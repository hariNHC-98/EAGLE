import numpy as np
import matplotlib.pyplot as plt

p = drone.DroneParamsAndMatrices()
p.load("/home/pieter/PO-EAGLE/Groups/ANC/Cleanup-Pieter/Code-Generators/ParamsAndMatrices/Output")
d = drone.Drone(p)
x0 = d.getStableState()

Q_att = np.diag((139.6245112700232,
                 139.6245112700232,
                 15.2811761590895,
                 1.1505204155597211,
                 1.1505204155597211,
                 0.1209919487616804,
                 9.976475759487083e-08,
                 9.976475759487083e-08,
                 9.976475759487083e-09))
R_att = np.diag((8, 8, 8))
K_pi_alt = np.array((0.0, 0.9, 0.5, -0.1))
maxIntegralInfluence = 0.1

controller = d.getController(Q_att, R_att, K_pi_alt, maxIntegralInfluence)

ref = drone.DroneReference()
ref.setOrientation((1, 0, 0, 0))

reff = drone.DroneReferenceFunction (lambda t: ref.asColVector())

odeopt = drone.AdaptiveODEOptions()
odeopt.t_start = 0
odeopt.t_end   = 16
odeopt.epsilon = 1e-6
odeopt.h_start = 1e-6
odeopt.h_min   = 1e-8
odeopt.maxiter = int(1e6)

result = d.simulate(controller, reff, x0, odeopt)
drone.plot(result)
plt.tight_layout()
plt.show()