import math


a = """
8.1402,-5.8022,-7.2376
10.1626,-7.4824,-6.5774
9.3153,-5.4974,-6.7025
9.7950,-5.2550,-9.1757
7.9095,-6.6488,-7.6088
10.3309,-5.3494,-5.9993
9.7758,-5.5840,-6.8061
10.6604,-4.9656,-5.9080
10.6105,-7.7490,-7.0800
7.0003,-6.4058,-8.7996
11.0574,-5.4305,-7.8553
10.5016,-7.9611,-8.1817
-5.4958,9.5486,5.3176
7.9991,-8.0350,-8.4103
"""



# Press the green button in the gutter to run the script.

def distance(p,q):
    dist=0
    for i in range(len(p)):
        dist+= (p[i]-q[i])**2
    return math.sqrt(dist)

def closest_centroid(x, centroids):
    min_distance =0
    closest = 0
    for i in range(len(centroids)):
        current_distance = distance(x,centroids[i])
        if current_distance < min_distance:
            min_distance=current_distance
            closest = i
    return closest

def mean(datapoints, i, k):
#gets a cluster and return the mean of all i placed arguments
    mean_i=0
    for x in range(len(datapoints)):
        mean_i+= datapoints[x][i]
    return mean_i/k

def update_centroid(centroids, cluster,k):
    new_cen=[]
    for c in range (len(centroids)):# for each centroid in the list, when every centroid is a datapoint in R^d
        for i in range(len(cluster[0])):
            new_cen[c][i]= mean(cluster[c],i,k)
    return new_cen

def centroid_dist(centroids, new_centroids):
    e=0.001
    for i in range(len(centroids)):
        if distance(centroids[i],new_centroids[i])>e:
            return False #not all centroids fill the condition
    return True #all centroids fill the condition


def main(k,input_data, iter=200 ):
    centroids = []
    cluster = []
    datapoints_side = []
    new_centroids = []
    cen_distance=False

    if (iter <= 1) or (iter >= 1000) or (iter %1 !=0):
        print("Invalid maximum iteration!")


    with open(input_data) as file:
        datapoints_side = file.readlines()
    datapoints_side = [x.strip() for x in datapoints_side] #remove line operator

    datapoints = []
    for d in datapoints_side:
        datapoints.append(d.split(','))


    for datapoint in range(len(datapoints)):
        for c in range(len(datapoints[0])):
            datapoints[datapoint][c] = float(datapoints[datapoint][c])


    if k<2 or k>=len(datapoints) or k%1!=0:
        print('Invalid number of clusters!')

    iteration_number= 0
    while cen_distance== False or iteration_number!=iter:
        for i in range(k):
            centroids.append(datapoints[i]) #initialize the centroid list

        for i in range(len(datapoints)): # split to clusters
            closest= closest_centroid(datapoints[i],centroids)
            print(cluster)
            cluster[closest].append(datapoints[i])

        new_centroids = update_centroid(centroids, cluster, k) #update centroids by current cluster
        cen_distance= centroid_dist(centroids,new_centroids)
        centroids= new_centroids
        iteration_number+1


if __name__ == '__main__':
    main(3, "data.txt", 600)