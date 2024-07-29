import sys, math

class Datapoint:
    def __init__(self, vec): #constructor
        self.vec = vec.copy()
        self.closest = None
        self.closest_distance = None
    
    def add(self, vec2): # add vec2 values to this vec
      for x in range(len(self.vec)):
        self.vec[x] += vec2.get_vec()[x]
    
    def divide(self, k): #divide all vec values by k
      	for x in range(len(self.vec)):
            self.vec[x] = self.vec[x]/k
    
    def copy(self, vec2): 
    	for x in range(len(self.vec)):
            self.vec[x] = vec2.get_vec()[x]
        
    def get_vec(self):
        return self.vec

    def delete_closest(self):
        self.closest=None
        self.closest_distance = None
    
    def change_closest(self, other_vec): # given a centroid, checks if its closer than the closest one yet and updates it accordingly
        distance = self.euclidian_distance(other_vec)
        if (self.closest is None or distance < self.closest_distance):
            self.closest = other_vec
            self.closest_distance = distance

    def get_closest(self):
        return self.closest

    def euclidian_distance(self, vec2): #calculate euc distance between this vec and the given one
        sum = 0
        for x in range(len(self.vec)):
            sum+= (self.vec[x] - vec2.get_vec()[x])**2
        
        return math.sqrt(sum)

    def __str__(self) -> str: #print the vectors in the given format
        a = ""
        for x in self.vec:
            a = a + '%.4f' % x+","
        return a[:-1]


def main():

    EPSILON = 0.001

    k = float(sys.argv[1])
    
    if (len(sys.argv)>3):
        iter = float(sys.argv[2])
        filename = sys.argv[3]
    else:
        iter = 200
        filename = sys.argv[2]
	
    if(k%1 != 0): #checks if k is a whole number
        print("Invalid number of clusters!")
        return 1
    k = int(k)

    if(iter%1 != 0): #checks if the number of wanted iterations is whole
        print("Invalid number of iteration!")
        return 1
    
    iter = int(iter)



    if(len(filename)<4 or filename[-4:]!=".txt"): #checks if the given file is indeed a txt file
        return 1

    
    if(iter <=1 or iter >= 1000): #checks if the given iteration number is valid
        print("Invalid maximum iteration!")
        return 1

    read = open(filename, "r")

    lines = read.read().split("\n") #split into vectors
    read.close()

    while(lines[-1] == ''):
        lines = lines[:-1] # delete last row (unwanted because the file format contains a blank line at the end)
    
    N = len(lines)
    
    if(k<=1 or k >= N):
        print("Invalid number of clusters!")
        return 1
    data_points_vals = []

    for line in lines:
        data_points_vals.append(line.split(",")) #split each vector to cords

    d = len(data_points_vals[0])

    for point in range(len(data_points_vals)):
        for cord in range(d):
            data_points_vals[point][cord] = float(data_points_vals[point][cord]) # convert to number

    data_points = []
    centroids = []

    for point in range(len(data_points_vals)):
        if point < k:
            centroids.append(Datapoint(data_points_vals[point])) # initialize centroids
        data_points.append(Datapoint(data_points_vals[point])) # initial datapoint


    found = False
    
    for x in range(iter):

        for point in data_points:
            point.delete_closest()
            for centroid in centroids:
                point.change_closest(centroid) #assign the closest centroid to each datapoint
            
        
        avg_point = Datapoint([0 for _ in range(d)]) # initialize empty datapoint
        count = 0
        for centroid in centroids:
            count = 0

            avg_point = Datapoint([0 for _ in range(d)])

            for point in data_points:
                if(point.get_closest() is centroid):
                    avg_point.add(point)
                    count+=1

            if(count>0):
                avg_point.divide(count) 
                if avg_point.euclidian_distance(centroid)>EPSILON:
                    found = True
                centroid.copy(avg_point)
            if (not found):
                break

    
    for x in centroids:
      print(x)
    return 0


if __name__ == "__main__":
    main()