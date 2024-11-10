import json

# Map of label names to YOLO class ids
label_map = {
    "blue_goal": 0,
    "yellow_goal": 1,
    # Add other labels here if needed
}

# List to hold YOLO formatted annotations
yolo_annotations = []
with open("Export v2 project - Goal Detection - 11_10_2024.ndjson", "r") as f: # Change this file to the path to the ndjson that you are using.
    data = [json.loads(line) for line in f]
# Example of processing one annotation
for annotation in data:  # Assuming 'data' is the list of annotations from your NDJSON
    # Get the image dimensions (height and width) from 'media_attributes'
    image_width = annotation['media_attributes']['width']
    image_height = annotation['media_attributes']['height']
    
    # Loop through the frames and their annotations
    for frame_number, frame_data in annotation['projects']['cm39qihof07kr07uo9u0a4si4']['labels'][0]['annotations']['frames'].items():
        # Iterate over the objects in the current frame
        for obj_id, obj in frame_data['objects'].items():
            # Extract label and bounding box
            label = obj["value"]  # This will be the "blue_goal"
            class_id = label_map[label]  # Map the label to YOLO class_id
            
            # Get bounding box values
            bbox = obj["bounding_box"]
            x_min = bbox["left"]
            y_min = bbox["top"]
            box_width = bbox["width"]
            box_height = bbox["height"]
            
            # Convert bounding box to YOLO format (normalized)
            x_center = (x_min + box_width / 2) / image_width
            y_center = (y_min + box_height / 2) / image_height
            width = box_width / image_width
            height = box_height / image_height
            
            # Store YOLO annotation in the format: class_id x_center y_center width height
            yolo_annotations.append(f"{class_id} {x_center} {y_center} {width} {height}")
            
# Print the YOLO annotations for verification
with open("yolo_annotations.txt", "w") as f:
    for annotation in yolo_annotations:
        f.write(f"{annotation}\n")
