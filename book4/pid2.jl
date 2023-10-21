#https://juliacontrol.github.io/ControlSystems.jl/v1.8/examples/example/#PID-plots

using LinearAlgebra
using ControlSystemsBase, Plots


h  = 0.1             # Sample time (only used for plots)
#x0     = [0.,0,0]    # Initial state
Tf     = 50              # Length of experiments (seconds)
t      = 0:h:Tf          # Time vector
tspan  = (0.0,Tf)

PID_KP = 0.9
PID_KI = 0.5
PID_KD = 0.0

setpoint = 2.0


#= c test
float TestSystem_Update(float inp) {
    float output;
     output = (1.0f + inp);
    return output;
}
=#
#https://matlab.mathworks.com/
#f=x+1  F=(s + 1)/s^2
#F=laplace(f) f=ilaplace(F)
P = tf([1,1],[1,0,0])  
ωp = 0.8
C1,kp,ki,fig = loopshapingPI(P,ωp,phasemargin=60,form=:parallel, doplot=true)
fig
#gangoffourplot(P, C)
y = timedomain()
plot(t,y')
#C1==pid(kp,ki,0;form=:parallel)

P = tf([1,1],[1,0,0])
C = pid(PID_KP, PID_KI, PID_KD; form=:parallel)
gangoffourplot(P, C)
#PC==CP
G=P*C/(1+P*C)
y1 = step(G |> ss, t).y
y = step(setpoint*G |> ss, t).y
si = stepinfo(setpoint*G, t)
plot(t,y')
plot!(t,y1')
plot(si)
pidplots(
    P,
    :controller;
    params_p = PID_KP,
    params_i = PID_KI,
    ω = ωp,
    ylims = (-2, 2),
    xlims = (-3, 3),
    form = :parallel,
)

L = feedback(P*C) |> ss
y = step(L, t).y
plot(t,y')

#=
res = step(G, t)
plot(res)
si = stepinfo(res)
plot(si)
=#

#=
res = step(P, t)
si = stepinfo(res, y0=0)
plot(si)
=#






#=
import sympy
//from sympy.abc import t, s, a
t, s = sympy.symbols('t, s')
a = sympy.symbols('a', real=True, positive=True)
f = t**a
F = sympy.laplace_transform(f, t, s, noconds=True)
F = exp(-a*s)/s
f = inverse_laplace_transform(, s, t)
=#