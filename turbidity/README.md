# turbidity

Using a simple optical sensor (as used in a washing machine) the turbidity of the water can be measured. Turbidity is an indicator of the dirtiness of the water. 

Turbidity itself cannot easily be measured. This particular sensor suffers from stray light influences and other error sources. There is no documentation on the relation between the measured voltage and the actual turbidity. So a quantitative measurement is not feasable.

However, we can measure changes in the turbidity. This short program measures mean and standard deviation of the output voltage and signals when the standard deviation rises over a threshold level.
