typedef struct tree {
	unsigned long key;
	index_t token, right, left, next;
}TTree;

int hash_key(unsigned char *str, unsigned long *hash) {

	args_assert(str != NULL, INTERNAL_ERROR);

	int c;

	*hash = 5381;

	while ((c = (*str)++))
		*hash = (((*hash) << 5) + *hash) + c;

	return RETURN_OK;
}

int add_symbol(TDynamic_structure_buffer *struct_buff_tokens, index_t token_to_store, TDynamic_structure_buffer *struct_buff_nodes, TDynamic_buff *b){

	index_t new_node_index;
	TTree *new_node;
	TToken *token;
	unsigned long key;

	new_item(struct_buff_nodes, new_node_index, new_node);

	if(new_node_index == 1){
		new_node->left = new_node->right = new_node->next = ZERO_INDEX;
		dereference_structure(struct_buff_tokens, token_to_store, &token);
		hash_key(load_token(b, token->token_index), &key);
		new_node->key = key;
		new_node->token_index = token_to_store;
	} else {
		create_node(struct_buff_tokens, token_to_store, struct_buff_nodes, 1, b, new_node_index);
	}

	return RETURN_OK;
}

int create_node(TDynamic_structure_buffer *struct_buff_tokens, index_t token_to_store, TDynamic_structure_buffer *struct_buff_nodes, index_t actual_node_index, TDynamic_buff *b, index_t new_node_index){

	TTree *node, *new_node;,
	TToken *token;
	unsigned long key;

	dereference_structure(struct_buff_nodes, actual_node_index, &node);
	dereference_structure(struct_buff_tokens, token_to_store, &token);
	hash_key(load_token(b, token->token_index), &key);

	if(key > node->key){
		if(node->right == ZERO_INDEX){
			dereference_structure(struct_buff_nodes, new_node_index, &new_node);
			node->right = new_node_index;
			new_node->left = new_node->right = new_node->next = ZERO_INDEX;
			new_node->key = key;
			new_node->token = token_to_store;
		}
		else
		{
			actual_node_index = node->right;
			create_node(struct_buff_tokens, token_to_store, struct_buff_nodes, actual_node_index, b, new_node_index);
		}
	}
	else if(key < node->key){
		if(node->left == ZERO_INDEX){
			dereference_structure(struct_buff_nodes, new_node_index, &new_node);
			node->left = new_node_index;
			new_node->right = new_node->left = new_node->next = ZERO_INDEX;
			new_node->key = key;
			new_node->token = token_to_store;
		}
		else
		{
			actual_node_index = node->left;
			create_node(struct_buff_tokens, token_to_store, struct_buff_nodes, actual_node_index, b, new_node_index);
		}
	}
	else if(key == node->key){
		while(node->next != ZERO_INDEX)
		{
			dereference_structure(struct_buff_nodes, node->next, &node);
		}
		node->next = new_node_index;
		dereference_structure(struct_buff_nodes, new_node_index, &new_node);
		new_node->right = new_node->left = new_node->next = ZERO_INDEX;
		new_node->key = key;
		new_node->token = token_to_store;
	}
	return RETURN_OK;
}
