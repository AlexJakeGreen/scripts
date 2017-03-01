package ua.kh.bars.endpoints;

import org.dozer.DozerBeanMapper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.ws.server.endpoint.annotation.Endpoint;
import org.springframework.ws.server.endpoint.annotation.PayloadRoot;
import org.springframework.ws.server.endpoint.annotation.RequestPayload;
import org.springframework.ws.server.endpoint.annotation.ResponsePayload;
import ua.kh.bars.entities.User;
import ua.kh.bars.services.UserService;
import ua.kh.bars.soap.*;

import java.util.List;

/**
 * Created by green on 4/10/16.
 */

@Endpoint
public class UserEndpoint {

    private static final String NS_URI = "http://bars.kh.ua/soap";

    @Autowired
    UserService userService;

    @Autowired
    DozerBeanMapper dozerBeanMapper;

    @PayloadRoot(namespace = NS_URI, localPart = "RegisterUserRequest")
    @ResponsePayload
    public RegisterUserResponse registerUser(@RequestPayload RegisterUserRequest request) {
        User user = userService.create(request.getName(), request.getEmail(), request.getUsername(), true);
        RegisterUserResponse response = new RegisterUserResponse();
        response.setUser(mapUserToJAXB(user));
        return response;
    }

    @PayloadRoot(namespace = NS_URI, localPart = "FindUserByIdRequest")
    @ResponsePayload
    public FindUserByIdResponse findUserById(@RequestPayload FindUserByIdRequest request) {
        User user = userService.findOne(request.getId());
        FindUserByIdResponse response = new FindUserByIdResponse();
        response.setUser(mapUserToJAXB(user));
        return response;
    }

    @PayloadRoot(namespace = NS_URI, localPart = "FindUserByEmailRequest")
    @ResponsePayload
    public FindUserByEmailResponse findUserByEmail(@RequestPayload FindUserByEmailRequest request) {
        User user = userService.findByEmail(request.getEmail());
        FindUserByEmailResponse response = new FindUserByEmailResponse();
        response.setUser(mapUserToJAXB(user));
        return response;
    }

    @PayloadRoot(namespace = NS_URI, localPart = "FindUserByNameRequest")
    @ResponsePayload
    public FindUserByNameResponse findUserByName(@RequestPayload FindUserByNameRequest request) {
        User user = userService.findByUsername(request.getName());
        FindUserByNameResponse response = new FindUserByNameResponse();
        response.setUser(mapUserToJAXB(user));
        return response;
    }

    @PayloadRoot(namespace = NS_URI, localPart = "FindUsersByNameRequest")
    @ResponsePayload
    public FindUsersByNameResponse findUsersByName(@RequestPayload FindUsersByNameRequest request) {
        List<User> users = userService.findUsersByName(request.getName());
        FindUsersByNameResponse response = new FindUsersByNameResponse();
        for(User u : users) {
            response.getUser().add(mapUserToJAXB(u));
        }
        return response;
    }

    // The first approach of mapping JAXB to Domain
    private ua.kh.bars.soap.User mapUserToJAXB(User user) {
        return dozerBeanMapper.map(user, ua.kh.bars.soap.User.class);
    }


}
