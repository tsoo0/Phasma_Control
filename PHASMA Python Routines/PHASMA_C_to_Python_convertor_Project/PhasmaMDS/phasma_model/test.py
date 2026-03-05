# import device_classes

from device_classes import Andor,ThomsonSettings


if __name__ == "__main__":
    
    test = Andor.Andor_iStar('a','b',1,2)
    
    a = ThomsonSettings.TSConfig(
        
        )