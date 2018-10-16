# ToxicDiagnose

The motivation of this project is to develope an expert system used for emergency room. Having a software to verify which poison patients possibly have would be very useful. By using a artificial intelligence system asking staff input symptoms, the software can easily searchs its knowledge base and gives out recommended treatments.



## Data Format
The data of symptoms and treatments of poisons is stored in [statement.txt](statement.txt).

## Sample Run
A good test case is to input the symptoms of **Colchicine**.

By inputing following inputs in order:

`No No Yes No No No Yes Yes` 

The software will use backward chaining to get the conclusion. Based on the conclusion, the software will then use forward chaining to find the appropriate treatment.

You will see the following sentences:
```
Recommended Treatment(s):
Adminster charcoal as slurry. Begin IV fluids. Gastric lavage within one hour of ingestion.
```

## Future improvement (todo)
Following things should be completed to make this software helpful and realistc enough to be a real world solution.

- The treatment options were coded in the program, which wasn't a good approach as it's not maintainable when the data gets large. The treatments were still few (15) at the moment. All data should be stored outside the program and be read as a file, and it's the priority for the future improvement.
- Improve the UI so it's more user-friendly and clear.
- Expanding the database so it supports more poisons and treatments.
- Develop the system for expanding the system so it's easier to generate the desired format of data.
- Adjust the system so it might also handle diseases/symptoms/disorders/damages ...etc instead of just poisons.

## Detailed Report
You can find more information about this software by checking [Report](Report.pdf)
