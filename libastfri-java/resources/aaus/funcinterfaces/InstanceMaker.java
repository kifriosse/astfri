/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Interface.java to edit this template
 */
package aaus_semestralka_1.pkg.funcinterfaces;

import java.util.ArrayList;

/**
 *
 * @author Adam-
 */
@FunctionalInterface
public interface InstanceMaker<T> 
{
    T makeInstance(ArrayList<String> values);
}
