import sys
from MDSplus import Tree
import os

tree_name = sys.argv[1]
shot = int(sys.argv[2])

# Optional: set MDS environment path if needed
# os.environ['MDSDIR'] = "C:\\MDSplus"

# Check if shot file exists
tree_file = os.path.join("C:\\MDSplus\\trees", f"{tree_name}", f"{tree_name}_{shot:05d}.dat")
if os.path.exists(tree_file):
    print(f"Shot {shot} already exists")
    sys.exit(1)

# Create new pulse safely
model_tree = Tree(tree_name, -1)  # open model tree
model_tree.createPulse(shot)
print(f"Shot {shot} created successfully")