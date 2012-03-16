#include "filters.hpp"

void highPassTimeStep(Circuit &c, int reduction){
    double dt = c.dt;
    double iR = c.iR;
    double iC = c.iC;
    int offset = reduction;
    for(int ip = 0; ip < c.Vin.rows; ip += offset){
        for(int jp = 0; jp < c.Vin.cols; jp += offset){
            int i = ip/offset;
            int j = jp/offset;
            for(int index = 0; index < 3;index++){
                double _Vout = c.Vout(i,j)[index];
                double _dVin = c.Vin(ip,jp)[index]-c.Vin_prev(ip,jp)[index];
                /* Low Pass */
                 
                double _dVout = c.dVout(i,j)[index];
                double _ddVout = ((c.Vin(ip,jp)[index]
                                    -c.Vin_prev(ip,jp)[index])
                                    -_dVout)*iR*iC/(dt*dt);
                /* High Pass */
//                double _dVout = _dVin-(c.iR*c.iC)*_dVout;
    
                c.Vout(i,j)[index] = _Vout+_dVout*dt;
                c.dVout(i,j)[index] = _dVout + _ddVout*dt;
                _Vout = c.Vin(ip,jp)[index]-c.qVout(i,j)[index];
                if(_Vout > 255)
                {
                    c.qVout(i,j)[index] = 255;
                }
                else if(_Vout < 0){
                    c.qVout(i,j)[index] = 0;
                }
                else
                    c.qVout(i,j)[index] = _Vout;
            }
        }
    }
}

void lowPassTimeStep(Circuit &c, int reduction){
    double dt = c.dt;
    double iR = c.iR;
    double iC = c.iC;
    int offset = reduction;
    for(int ip = 0; ip < c.Vin.rows; ip += offset){
        for(int jp = 0; jp < c.Vin.cols; jp += offset){
            int i = ip/offset;
            int j = jp/offset;
            for(int index = 0; index < 3;index++){
                double _Vout = c.Vout(i,j)[index];
                double _dVin = c.Vin(ip,jp)[index]-c.Vin_prev(ip,jp)[index];
                /* Low Pass */
                 
                double _dVout = c.dVout(i,j)[index];
                double _ddVout = ((c.Vin(ip,jp)[index]
                                    -c.Vin_prev(ip,jp)[index])
                                    -_dVout)*iR*iC/(dt*dt);
                /* High Pass */
//                double _dVout = _dVin-(c.iR*c.iC)*_dVout;
    
                c.Vout(i,j)[index] = _Vout+_dVout*dt;
                c.dVout(i,j)[index] = _dVout + _ddVout*dt;
                _Vout = c.Vin(ip,jp)[index]-c.qVout(i,j)[index];
                if(_Vout > 255)
                {
                    c.qVout(i,j)[index] = 255;
                }
                else if(_Vout < 0){
                    c.qVout(i,j)[index] = 0;
                }
                else
                    c.qVout(i,j)[index] = _Vout;
            }
        }
    }
}
