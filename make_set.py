import csv
import random
import sys

def generate_random_points(num_points, min_x, max_x, min_y, max_y):
    """
    generate_random_points function generates a specified number of points within the given range of coordinate values.
    """
    points = []
    for _ in range(num_points):
        x = random.randint(min_x, max_x)
        y = random.randint(min_y, max_y)
        points.append((x, y))
    return points

def write_points_to_csv(points, filename):
    """
    write_points_to_csv function takes the list of points generated by the generate_random_points function and makes a .csv file out of it.
    """
    with open(filename, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        for point in points:
            writer.writerow(point)

num_points = int(sys.argv[1])
min_x, max_x = 10, 1190
min_y, max_y = 10, 690
points = generate_random_points(num_points, min_x, max_x, min_y, max_y)
write_points_to_csv(points, 'points.csv')
