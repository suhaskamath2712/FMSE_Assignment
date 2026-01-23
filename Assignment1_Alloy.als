//====== Initial model ======
sig Car {}

one sig Rentals
{
	available: set Car, //set of available cars
	rented: set Car
}

//====== Problem 1 ======
pred p1 
{
	//P1: At any instant, every car is either available or rented
	all c: Car | c in Rentals.available + Rentals.rented

	//no car can be in both available and rented sets at the same time
	no c: Car | c in Rentals.available and c in Rentals.rented
}

pred p2
{
	//P2: There is always at least one available car
	some Rentals.available
}

pred badInstances
{
	!(p1 and p2)
}

run badInstances for 8

//====== Problem 2 ======
fact p1_and_p2
{
	p1 and p2
}

pred Show {}

run Show for 8


//====== Problem 3 ======
//Ensures P1 & P2 are satisfied
pred original_ok
{
	all c: Car | c in Rentals.available + Rentals.rented
	and some Rentals.available
}

//Precondition for rent: P1 & P2 must hold, toRent must be available, and at least one available car must be there after renting
pred pre_rent[toRent: Car]
{
	original_ok
	and toRent in Rentals.available
	and some (Rentals.available - toRent) // at least one available remains after renting
}

pred rent[toRent: Car, newAvail : Rentals -> set Car, newRented : Rentals -> set Car]
{
	//If the precondition is satisfied, rent the car
	pre_rent[toRent]
	implies
		newAvail = Rentals -> (Rentals.available - toRent) and
		newRented = Rentals -> (Rentals.rented + toRent)
}

run rent for 8

// Precondition for return: P1 & P2 must hold and toReturn must be currently rented.
pred pre_return[toReturn: Car]
{
	original_ok
	and toReturn in Rentals.rented
}

pred return[toReturn: Car, newAvail : Rentals -> set Car, newRented : Rentals -> set Car]
{
	//If the precondition is satisfied, return the car
	pre_return[toReturn]
	implies
		newAvail = Rentals -> (Rentals.available + toReturn) and
		newRented = Rentals -> (Rentals.rented - toReturn)
}

run return for 8


//====== Problem 4 ======
// Checking predicate for rent
pred findBugsInrent[toRent: Car, newAvail : Rentals -> set Car, newRented : Rentals -> set Car]
{
	// Ensure the operation is applicable
	pre_rent[toRent]
	and rent[toRent, newAvail, newRented]

	// Post-state violates at least one of P1 or P2
	and (
		// Violation of P1: some car is neither available nor rented in the new state
		some c: Car | c not in (newAvail[Rentals] + newRented[Rentals])
		// OR Violation of P2: no available cars in the new state
		or no newAvail[Rentals]
	)
}

// Checking predicate for return
pred findBugsInreturn[toReturn: Car, newAvail : Rentals -> set Car, newRented : Rentals -> set Car]
{
	// Ensure the operation is applicable
	pre_return[toReturn]
	and return[toReturn, newAvail, newRented]

	// Post-state violates at least one of P1 or P2
	and (
		some c: Car | c not in (newAvail[Rentals] + newRented[Rentals])
		or no newAvail[Rentals]
	)
}

run findBugsInrent for 8

run findBugsInreturn for 8